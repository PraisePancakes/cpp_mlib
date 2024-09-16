#pragma once

template <typename __Ty>
class allocator
{
    __Ty *_M_pfirst;

public:
    size_t _Alloc_cell_size;
    size_t _Alloc_capacity_size;
    size_t _Alloc_capacity_byte_size;

    allocator()
    {
        _Alloc_cell_size = sizeof(__Ty);
        _Alloc_capacity_size = 1;
        _Alloc_capacity_byte_size = _Alloc_cell_size * _Alloc_capacity_size;
    };
    __Ty *allocate(const size_t __alloc_size__)
    {
        _Alloc_capacity_size = __alloc_size__;
        const size_t new_size = _Alloc_capacity_size * _Alloc_cell_size;
        _Alloc_capacity_byte_size = new_size;
        _M_pfirst = (__Ty *)malloc(new_size);
        return _M_pfirst;
    };

    __Ty *reallocate(const size_t __realloc_size__)
    {
        _Alloc_capacity_size = __realloc_size__;
        const size_t new_size = _Alloc_capacity_size * _Alloc_cell_size;
        _Alloc_capacity_byte_size = new_size;
        _M_pfirst = (__Ty *)realloc(_M_pfirst, new_size);
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