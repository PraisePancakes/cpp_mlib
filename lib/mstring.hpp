#pragma once
#include "mallocator.hpp"
#include "miterator.hpp"
#include <cstddef>
#include <iostream>
#define _DEF_STR_CAPACITY 1
#define _AMORT_FAC 2
#define _SSO_DEOPTIMIZER_THRESHOLD 10

namespace mlib
{

    template <typename T>
    class char_traits;

    template <>
    class char_traits<char>
    {

    public:
        typedef char char_type;
        typedef int int_type;
        typedef const char_type &const_reference;

        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

        [[nodiscard]] static constexpr size_t length(const char_type *_data_) noexcept
        {
            size_t count = 0;
            while (*_data_ != char_type())
            {
                ++count;
                ++_data_;
            };
            return count;
        };

        [[nodiscard]] static constexpr bool eq(const char_type a, const char_type b) noexcept
        {
            return static_cast<unsigned char>(a) == static_cast<unsigned char>(b);
        };

        [[nodiscard]] static constexpr bool lt(const char_type a, const char_type b) noexcept
        {
            return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
        };

        [[nodiscard]] static constexpr bool gt(const char_type a, const char_type b) noexcept
        {
            return !lt(a, b);
        };

        static char_type *copy(char_type *const _dest_, const char_type *const _src_, size_t _n_) noexcept
        {
            for (size_t i = 0; i < _n_; ++i)
            {
                _dest_[i] = _src_[i];
            }
            return _dest_;
        }
    };

    template <>
    class char_traits<wchar_t>
    {

    public:
        typedef wchar_t char_type;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;

        [[nodiscard]] static constexpr size_t length(const char_type *_data_) noexcept
        {
            size_t count = 0;
            while (*_data_ != char_type())
            {
                ++count;
                ++_data_;
            };
            return count;
        };

        [[nodiscard]] static constexpr bool eq(const char_type a, const char_type b) noexcept
        {
            return static_cast<unsigned char>(a) == static_cast<unsigned char>(b);
        };

        [[nodiscard]] static constexpr bool lt(const char_type a, const char_type b) noexcept
        {
            return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
        };

        [[nodiscard]] static constexpr bool gt(const char_type a, const char_type b) noexcept
        {
            return !lt(a, b);
        };

        static char_type *copy(char_type *const _dest_, const char_type *const _src_, size_t _n_) noexcept
        {
            for (size_t i = 0; i != _n_; ++i)
            {
                _dest_[i] = _src_[i];
            }
            return _dest_;
        }
    };

    template <typename T, typename CTraits, typename Alloc>
    class str_base
    {

    private:
        typedef CTraits _traits;
        typedef allocator_traits<Alloc> allocator_traits;
        typedef allocator_traits::value_type char_type;
        typedef T *pointer;
        typedef T &reference;
        typedef allocator_traits::const_pointer const_pointer;

        typedef _traits::size_type size_type;
        typedef _traits::difference_type difference_type;

        union m_Final_impl
        {
            char_type sso_buff[_SSO_DEOPTIMIZER_THRESHOLD];
            pointer m_heap_region;
        };

        bool m_sso_optimized;

        void _init_impl(pointer _buffer_, const_pointer _src_, size_type _size_, size_type _capacity_)
        {
            this->m_region_start = &_buffer_[0];
            this->m_region_end = &_buffer_[0 + _size_];
            this->m_region_capacity = &_buffer_[_capacity_];
            _traits::copy(_buffer_, _src_, _size_);
            this->m_region_start[_size_] = '\0';
        };

    protected:
        Alloc m_allocator;
        pointer m_region_start;
        pointer m_region_end;
        pointer m_region_capacity;
        m_Final_impl f_impl;

    public:
    public:
        str_base() : m_region_start(nullptr), m_region_end(nullptr), m_region_capacity(nullptr), m_sso_optimized(true)
        {
            f_impl.sso_buff[0] = '\0';
        }

        str_base(const_pointer _src_, size_type _size_)
        {

            if (_size_ < _SSO_DEOPTIMIZER_THRESHOLD)
            {
                _init_impl(f_impl.sso_buff, _src_, _size_, _SSO_DEOPTIMIZER_THRESHOLD);
                m_sso_optimized = true;
            }
            else
            {
                size_type capacity = _size_ * _AMORT_FAC;
                f_impl.m_heap_region = m_allocator.allocate(capacity);
                _init_impl(f_impl.m_heap_region, _src_, _size_, capacity);
                m_sso_optimized = false;
            }
        };

        size_type capacity() const
        {
            return this->m_region_capacity - this->m_region_start;
        };

        char_type *data() const
        {
            return this->m_region_start;
        };

        size_type size() const
        {
            return char_traits<T>::length(m_region_start);
        };

        size_type length() const
        {
            return size();
        }

        ~str_base()
        {
            if (!this->m_sso_optimized)
            {
                m_allocator.destroy(this->f_impl.m_heap_region);
            };
        };
    };

    template <typename T,
              typename CTraits = char_traits<T>,
              typename Alloc = allocator<T>>
    class basic_string : public str_base<T, CTraits, Alloc>
    {

        typedef CTraits _traits;

        size_t _calculate_amortized_growth()
        {
            return ((this->m_region_capacity - this->m_region_start + 1) << 1);
        };

        void _resize_by_offset(const size_t _capacity_size_offset_)
        {
            size_t current_size = this->m_region_end - this->m_region_start;

            size_t current_capacity = this->m_region_capacity - this->m_region_start;
            size_t new_capacity = current_capacity + _capacity_size_offset_;
            typename Alloc::pointer new_start = this->m_allocator.reallocate(this->m_region_start, new_capacity);

            if (!new_start)
            {
                throw std::bad_alloc();
            }

            if (new_start != this->m_region_start)
            {
                this->m_region_start = new_start;
                this->m_region_end = new_start + current_size;
                this->m_region_capacity = new_start + new_capacity;
            }
        }

        template <typename U>
        class impl_string_iterator
        {
            using value_type = U;
            using pointer = U *;
            using const_pointer = const U *;
            using reference = U &;
            using const_reference = const U &;
            using category = bidirectional_tag;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;
            using this_it = impl_string_iterator<U>;

            pointer m_Iterator;

        public:
            impl_string_iterator() : m_Iterator(nullptr) {};
            impl_string_iterator(pointer _loc_) : m_Iterator(_loc_) {};

            this_it &operator++()
            {
                m_Iterator++;
                return *this;
            };

            this_it &operator--()
            {
                m_Iterator--;
                return *this;
            }

            this_it operator++(int)
            {
                this_it temp = *this;
                ++(*this);
                return temp;
            }

            this_it operator--(int)
            {
                this_it temp = *this;
                --(*this);
                return temp;
            }

            reference operator*()
            {
                return *m_Iterator;
            };

            bool operator==(const this_it &_other_)
            {
                return this->m_Iterator == _other_.m_Iterator;
            }

            bool operator!=(const this_it &_other_)
            {
                return !((*this) == _other_);
            };

            ~impl_string_iterator() {};
        };

        using iterator = impl_string_iterator<T>;
        using const_iterator = impl_string_iterator<const T>;
        using reverse_iterator = mlib::reverse_iterator<iterator>;
        using const_reverse_iterator = mlib::reverse_iterator<const_iterator>;

    public:
        basic_string() : str_base<T, CTraits, Alloc>("", 0) {

                         };

        basic_string(const T *_src_)
            : str_base<T, CTraits, Alloc>(_src_, char_traits<T>::length(_src_)) {

              };

        basic_string(const basic_string &_other_)
            : str_base<T, CTraits, Alloc>(_other_.data(), char_traits<T>::length(_other_.data())) {};

        basic_string &operator=(const basic_string &_other_)
        {
            if (_other_.size() >= this->capacity())
            {
                this->_resize_by_offset(_other_.size() - this->capacity());
            }

            _traits::copy(this->m_region_start, _other_.data(), _other_.size());
            this->m_region_start[_other_.size()] = '\0';
            this->m_region_end = this->m_region_start + _other_.size();
            return *this;
        };

        basic_string(const _traits::size_type _size_) : str_base<T, CTraits, Alloc>("", _size_) {};

        void push_back(typename Alloc::const_reference _v_)
        {

            if (this->size() >= this->capacity())
            {

                this->_resize_by_offset(this->_calculate_amortized_growth());
            }

            std::ptrdiff_t size = this->m_region_end - this->m_region_start;

            this->m_allocator.construct(this->m_region_start + size, _v_);

            ++this->m_region_end;

            *(this->m_region_end) = '\0';
        }

        iterator begin()
        {
            return iterator(this->m_region_start);
        };

        iterator end()
        {
            return iterator(this->m_region_end);
        };

        void push_back(typename Alloc::reference &_v_)
        {
            if (this->size() >= this->capacity())
            {

                this->_resize_by_offset(this->_calculate_amortized_growth());
            }

            this->m_allocator.construct(this->m_region_start + this->size(), _v_);
            this->m_region_end++;
        }

        void insert(typename _traits::size_type _i_, typename _traits::const_reference _v_)
        {

            if (_i_ >= this->size())
            {

                push_back(_v_);
            }
            else
            {
                typename _traits::size_type si = _i_;
                typename _traits::size_type e = this->size() + 1;

                while (e > si)
                {

                    *(this->m_region_start + e) = *(this->m_region_start + (e - 1));
                    e--;
                }

                *(this->m_region_start + e) = _v_;
                this->m_region_end++;
            }
        };

        void reverse()
        {
            if (this->size() == 0)
                return;

            if (this->size() == 1)
                return;

            typename _traits::size_type j = this->size() - 1;
            for (typename _traits::size_type i = 0; i < j; i++)
            {
                const typename _traits::char_type temp = this->m_region_start[i];
                this->m_region_start[i] = this->m_region_start[j];
                this->m_region_start[j] = temp;
                j--;
            }
        };

        void reverse(typename _traits::size_type _start_, typename _traits::size_type _end_)
        {
            // reverse from start to end.
            if (this->size() == 0)
                return;

            if (this->size() == 1)
                return;

            const typename _traits::size_type temp_start = _start_;
            const typename _traits::size_type temp_end = _end_;

            if (_start_ > _end_)
            {
                _start_ = temp_end;
                _end_ = temp_start;
            }

            typename _traits::size_type j = _end_;
            for (typename _traits::size_type i = _start_; i < j; i++, j--)
            {
                const typename _traits::char_type temp = this->m_region_start[i];
                this->m_region_start[i] = this->m_region_start[j];
                this->m_region_start[j] = temp;
            }
        };

        void splice(typename _traits::size_type _s_, typename _traits::size_type _n_deletes_, std::initializer_list<typename _traits::char_type> _elems_)
        {
            const typename _traits::size_type start = _s_;
            typename _traits::size_type span = _n_deletes_;

            if (span > _elems_.size())
            {
                typename _traits::size_type i = start;
                for (typename _traits::size_type index = 0; index < _elems_.size(); index++)
                {
                    *(this->m_region_start + i) = _elems_.begin()[index];
                    i++;
                }

                typename _traits::size_type _n_lshift = span - _elems_.size();

                i = start + _elems_.size();

                for (typename _traits::size_type iter = 0; iter < _n_lshift; iter++)
                {
                    for (typename _traits::size_type j = start + _elems_.size(); j < this->size(); j++)
                    {

                        this->m_region_start[j] = this->m_region_start[j + 1];
                    }

                    this->m_region_end--;
                }
            }
            else if (span < _elems_.size())
            {

                typename _traits::size_type list_cursor = 0;
                typename _traits::size_type i = start;

                for (; i < start + span; ++i)
                {

                    this->m_region_start[i] = _elems_.begin()[list_cursor];
                    list_cursor++;
                }

                for (typename _traits::size_type j = i; j < start + _elems_.size(); j++)
                {

                    insert(j, _elems_.begin()[list_cursor]);
                    list_cursor++;
                }
            }
            else if (span == _elems_.size())
            {
                typename _traits::size_type list_cursor = 0;
                for (typename _traits::size_type i = start; i < start + span; ++i)
                {
                    this->m_region_start[i] = _elems_.begin()[list_cursor];
                    list_cursor++;
                }
            }
        };

        mlib::basic_string<typename _traits::char_type> slice(typename _traits::size_type __start__, typename _traits::size_type __end__)
        {
            const int temp_start = __start__;
            const int temp_end = __end__;

            if (__start__ < 0)
            {
                __start__ = 0;
            }

            if (__end__ >= this->size())
            {
                __end__ = this->size();
            }

            if (__start__ > __end__)
            {
                __end__ = temp_start;
                __start__ = temp_end;
            }

            mlib::basic_string<typename _traits::char_type> s(__end__ - __start__);

            for (typename _traits::size_type i = __start__; i < __end__; i++)
            {
                this->m_allocator.construct(s.m_region_start + i, *(this->m_region_start + i));
                s.m_region_end++;
            }

            return s;
        }

        T &operator[](size_t _index_) const
        {
            return this->m_region_start[_index_];
        };

        friend std::ostream &operator<<(std::ostream &_os_, const basic_string &_str_)
        {
            std::cout << _str_.data();
            return _os_;
        };
    };

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;

    namespace MLIB_STRING_IMPL_HELPER
    {
        template <typename T>
        basic_string<T> concat(const T *_lhs_, const T *_rhs_, size_t new_size)
        {

            const size_t lhs_size = mlib::char_traits<T>::length(_lhs_);
            const size_t rhs_size = new_size - lhs_size;

            basic_string<T> new_string(new_size);

            char_traits<T>::copy(new_string.data(), _lhs_, lhs_size);
            char_traits<T>::copy(new_string.data() + lhs_size, _rhs_, rhs_size);

            return new_string;
        };
    };

    template <typename T>
    basic_string<T> operator+(const basic_string<T> &lhs, const basic_string<T> &rhs)
    {
        return MLIB_STRING_IMPL_HELPER::concat(lhs.data(), rhs.data(), lhs.size() + rhs.size());
    };

    template <typename T>
    basic_string<T> operator+(const basic_string<T> &lhs, const char *rhs)
    {
        return MLIB_STRING_IMPL_HELPER::concat(lhs.data(), rhs, lhs.size() + mlib::char_traits<T>::length(rhs));
    };

};
