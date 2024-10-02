#pragma once
#include "mallocator.hpp"
#define _DEF_STR_CAPACITY 1
#define _AMORT_FAC 2

namespace mlib
{

    template <typename T>
    class char_traits;

    template <>
    class char_traits<char>
    {

    public:
        typedef char char_type;
        typedef std::ptrdiff_t difference_type;
        typedef std::size_t size_type;

        static size_t length(const char_type *const _data_)
        {
            if (_data_ == nullptr)
            {
                return 0;
            }
            size_t len = 0;
            const char_type *pdata = _data_;

            while (*pdata != '\0')
            {
                len++;
                pdata++;
            };

            pdata = nullptr;
            return len;
        };

        static constexpr bool eq(char_type a, char_type b) noexcept
        {
            return a == b;
        };

        static char *copy(char_type *const _dest_, const char_type *_src_, size_t _n_)
        {
            char_type *pdest = _dest_;

            size_t safe_index = 0;
            for (size_t i = 0; i < _n_; i++)
            {
                pdest[i] = _src_[safe_index];
                safe_index++;
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
        typedef std::size_t size_type;

        static size_t length(const char_type *const _data_)
        {
            if (_data_ == nullptr)
            {
                return 0;
            }
            size_t len = 0;
            const char_type *pdata = _data_;

            while (*pdata != '\0')
            {
                len++;
                pdata++;
            };

            pdata = nullptr;
            return len + 1;
        };

        static constexpr bool eq(char_type a, char_type b) noexcept
        {
            return a == b;
        };

        static char_type *copy(char_type *const _dest_, const char_type *_src_, size_t _n_)
        {
            char_type *pdest = _dest_;

            size_t safe_index = 0;
            for (size_t i = 0; i < _n_; i++)
            {
                pdest[i] = _src_[safe_index];
                safe_index++;
            }

            return _dest_;
        }
    };

    template <typename T, typename _CTraits, typename _Alloc>
    class str_base
    {

    protected:
        typedef _CTraits _traits;
        typedef T char_type;
        typedef T *pointer;
        typedef T &reference;
        typedef const T *const_pointer;

        typedef _traits::size_type size_type;
        typedef _traits::difference_type difference_type;

        _Alloc _M_allocator;

        struct impl_data
        {
            pointer _M_region_start;
            pointer _M_region_end;
            pointer _M_region_capacity;

            impl_data() : _M_region_capacity(nullptr), _M_region_end(nullptr), _M_region_start(nullptr) {};
            impl_data(pointer _s_, pointer _e_, pointer _c_) : _M_region_start(_s_), _M_region_capacity(_c_), _M_region_end(_e_) {};
            impl_data(impl_data &&_other_) : _M_region_start(_other_._M_region_start),
                                             _M_region_end(_other_._M_region_end),
                                             _M_region_capacity(_other_._M_region_capacity)
            {
                _other_._M_region_capacity = _other_._M_region_start = _other_._M_region_end = pointer();
            }

            impl_data(const impl_data &_other_)
            {
                _M_region_capacity = _other_._M_region_capacity;
                _M_region_end = _other_._M_region_end;
                _M_region_start = _other_._M_region_start;
            }

            impl_data &operator=(const impl_data &_other_)
            {
                _M_region_capacity = _other_._M_region_capacity;
                _M_region_end = _other_._M_region_end;
                _M_region_start = _other_._M_region_start;
                return *this;
            }

            void _copy_data(const impl_data &_other_)
            {
                _M_region_capacity = _other_._M_region_capacity;
                _M_region_end = _other_._M_region_end;
                _M_region_start = _other_._M_region_start;
            };

            void _copy_data(pointer _s_, pointer _c_)
            {
                if (_s_ == nullptr)
                {
                    std::cout << "null";
                }

                if (_c_ == nullptr)
                {
                    std::cout << "null";
                }

                _M_region_start = _s_;
                _M_region_end = _s_;
                _M_region_capacity = _c_;
            };

            ~impl_data()
            {
                _M_region_start = nullptr;
                _M_region_capacity = nullptr;
                _M_region_end = nullptr;
            };
        };

        impl_data _M_impl;

    public:
        str_base() {};

        str_base(const str_base &_other_)
        {
            this->_M_impl = _other_._M_impl;
        };
        str_base(str_base &&_other_)
        {
            this->_M_impl = _other_._M_impl;
        };

        ~str_base() {};
    };

    template <typename T,
              typename _CTraits = char_traits<T>,
              typename _Alloc = allocator<T>>
    class basic_string : public str_base<T, _CTraits, _Alloc>
    {

        typedef _CTraits _traits;
        typedef allocator_traits<T> alloc_traits;
        typedef str_base<T, _CTraits, _Alloc> base;
        typedef typename base::pointer pointer;
        typedef typename base::const_pointer const_pointer;
        typedef typename base::size_type size_type;
        typedef typename base::difference_type difference_type;

        base _M_base;
        typename base::impl_data _M_impl;
        void _fill_buffer(pointer const buff, const_pointer _src_, size_type size)
        {
            size_type safe_i = 0;
            for (size_type i = 0; i < size; i++)
            {
                *(buff + i) = *(_src_ + safe_i);
                safe_i++;
            }
        }

    public:
        basic_string() {

        };

        basic_string(const T *_src_)
        {
            size_type size = char_traits<T>::length(_src_);
            size_type capacity = size * _AMORT_FAC;
            pointer start = this->_M_allocator.allocate(capacity);
            this->_M_impl._copy_data(start, start + capacity);
            _fill_buffer(this->_M_impl._M_region_start, _src_, size);
        };

        size_type size() const noexcept
        {
            return char_traits<T>::length(_M_impl._M_region_start);
        };

        size_type length() const noexcept
        {
            return size();
        };
    };

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
};