#pragma once
#include "mallocator.hpp"
#define _DEF_STR_CAPACITY 1

namespace mlib
{

    template <typename _CharType>
    class char_traits;

    template <>
    class char_traits<char>
    {

    public:
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
        typedef _CharType char_type;
        typedef _CTraits char_traits;
        typedef _Alloc allocator_type;
        typedef _CharType *pointer;
        typedef const _CharType *const_pointer;
        typedef char_type &reference;
        typedef const char_type &const_reference;

        typedef std::ptrdiff_t difference_type;
        typedef std::size_t size_type;

        allocator_type _M_allocator;

    public:
        basic_string() {
            
        };
    };

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
};