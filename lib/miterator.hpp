#pragma once
#include <cstddef>
namespace mlib
{
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
