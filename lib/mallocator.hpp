#pragma once

template <typename __Ty>
class allocator
{
    __Ty *_M_pfirst;

public:
    allocator() {};
    __Ty *allocate(const size_t __alloc_size__)
    {
        _M_pfirst = (__Ty *)malloc(__alloc_size__ * sizeof(__Ty));
        return _M_pfirst;
    };

    __Ty *reallocate(const size_t __realloc_size__)
    {
        _M_pfirst = (__Ty *)realloc(_M_pfirst, __realloc_size__ * sizeof(__Ty));
        return _M_pfirst;
    }

    ~allocator() {};
};

/*
    template<typename _Ty, class Allocator = allocator<_Ty>>
    class vec {
        _Ty _M_pfirst
        _Allocator
    };
*/