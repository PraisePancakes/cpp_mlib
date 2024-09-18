#pragma once
#include "mallocator.hpp"

namespace mlib
{

    template <typename _CharType>
    class char_traits;

    template <>
    class char_traits<char>
    {
        const char *_M_p_underlying;

    public:
        char_traits(const char *const _data_)
        {
            this->_M_p_underlying = _data_;
        };

        char_traits()
        {
            this->_M_p_underlying = nullptr;
        };

        static size_t length(const char *const _data_)
        {
            if (_data_ == nullptr)
            {
                return 0;
            }
            size_t len = 0;
            const char *pdata = _data_;

            while (*pdata != '\0')
            {
                len++;
                pdata++;
            };

            pdata = nullptr;
            return len;
        };

        static constexpr bool eq(char a, char b) noexcept
        {
            return a == b;
        };

        static char *copy(char *const _dest_, const char *_src_, size_t _n_)
        {
            char *pdest = _dest_;

            size_t safe_index = 0;
            for (size_t i = 0; i < _n_; i++)
            {
                pdest[i] = _src_[safe_index];
                safe_index++;
            }

            return _dest_;
        }

        const char *data() const noexcept
        {
            return _M_p_underlying;
        }
    };

    template <>
    class char_traits<wchar_t>
    {
    public:
        char_traits() {};
    };

    template <typename _CharType,
              typename _CTraits = char_traits<_CharType>,
              typename _Alloc = allocator<_CharType>>
    class basic_string
    {
        _Alloc _M_allocator;
        _CTraits _M_traits;
        _CharType *_M_dc_string;
        size_t _M_size;

        _CharType *_dc_string(const _CharType *_str_, const size_t _len_)
        {
            _CharType *allocated = _M_allocator.allocate(_len_);
            char_traits<_CharType>::copy(allocated, _str_, _len_);
            return allocated;
        };

    public:
        basic_string(const _CharType *_str_)
        {
            _M_traits = _str_;
            _M_size = char_traits<_CharType>::length(_str_);
            _M_dc_string = _dc_string(_str_, _M_size);
        };

        basic_string(const basic_string<_CharType> &_other_)
        {
            this->_M_traits = _other_._M_dc_string;
            this->_M_size = _other_.size();
            this->_M_dc_string = _dc_string(_other_._M_dc_string, _other_.size());
        }

        size_t size() const
        {
            return _M_size;
        }

        size_t length() const
        {
            return _M_size;
        }

        basic_string() {};
        friend std::ostream &operator<<(std::ostream &_os_, basic_string<_CharType> _bstring_)
        {
            return _os_ << _bstring_._M_dc_string;
        }
        ~basic_string()
        {
            delete _M_dc_string;
        };
    };

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
};