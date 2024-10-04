#pragma once
#include "mallocator.hpp"
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
        pointer _M_region_start;
        pointer _M_region_end;
        pointer _M_region_capacity;

        union _M_Final_impl
        {
            char_type sso_buff[_SSO_DEOPTIMIZER_THRESHOLD];
            pointer _M_heap_region;
        };

        bool _M_sso_optimized;

        void _fill_buffer(pointer buff, const_pointer _src_, size_type size)
        {

            size_type safe_i = 0;
            for (size_type i = 0; i < size; i++)
            {
                *(buff + i) = *(_src_ + safe_i);
                safe_i++;
            }
        }

    public:
        _M_Final_impl f_impl;

        str_base() : _M_region_start(nullptr), _M_region_end(nullptr), _M_region_capacity(nullptr), _M_sso_optimized(true)
        {
            f_impl.sso_buff[0] = '\0';
        }

        void _init_impl(pointer _buffer_, const_pointer _src_, size_type _size_, size_type _capacity_)
        {
            this->_M_region_start = &_buffer_[0];
            this->_M_region_end = &_buffer_[0 + _size_];
            this->_M_region_capacity = &_buffer_[_capacity_];
            _fill_buffer(_buffer_, _src_, _size_);

            _M_region_start[_size_] = '\0';
        };

        str_base(const_pointer _src_, size_type _size_)
        {
            if (_size_ <= _SSO_DEOPTIMIZER_THRESHOLD)
            {
                _init_impl(f_impl.sso_buff, _src_, _size_, _SSO_DEOPTIMIZER_THRESHOLD);
                _M_sso_optimized = true;
            }
            else
            {
                size_type capacity = _size_ * _AMORT_FAC;
                f_impl._M_heap_region = _M_allocator.allocate(capacity);
                _init_impl(f_impl._M_heap_region, _src_, _size_, capacity);
                _M_sso_optimized = false;
            }
        };

        size_type _inner_size() const
        {
            return char_traits<T>::length(_M_region_start);
        };

        ~str_base()
        {
            if (!this->_M_sso_optimized)
            {
                _M_allocator.destroy(this->f_impl._M_heap_region);
            };
        };
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

    public:
        basic_string() : str_base<T, _CTraits, _Alloc>("", 0) {

                         };

        basic_string(const T *_src_) : str_base<T, _CTraits, _Alloc>(_src_, char_traits<T>::length(_src_)) {

                                       };

        size_type size() const noexcept
        {
            return this->_inner_size();
        };

        size_type length() const noexcept
        {
            return size();
        };

        friend std::ostream &operator<<(std::ostream &_os_, const basic_string &_str_)
        {
            if (_str_.size() < _SSO_DEOPTIMIZER_THRESHOLD)
            {
                std::cout << _str_.f_impl.sso_buff;
            }
            else
            {
                std::cout << _str_.f_impl._M_heap_region;
            }

            return _os_;
        };
    };

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
};