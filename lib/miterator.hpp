#pragma once
#include <cstddef>

namespace mlib
{
    template <typename _Ty>
    struct iterator_traits
    {
        typedef _Ty value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;
    };

    template <typename _Ty>
    struct iterator_traits<_Ty *> // int* , char* etc..
    {
        typedef _Ty value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
    };

    template <class _ITCategory, class _T, class _ITDistance = std::ptrdiff_t, class _ITPointer = _T *, class _ITConstPointer = const _T *>
    struct iterator
    {
        typedef _T value_type;
        typedef _ITCategory category;
        typedef _ITDistance difference_type;
        typedef _ITPointer pointer;
        typedef _ITConstPointer const_pointer;
    };

}
