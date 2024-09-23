#pragma once
#include <cstddef>

namespace mlib
{
    struct random_access_tag
    {
    };

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
        typedef size_t difference_type;
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

    template <class _Iter>
    struct normal_iterator : public iterator<random_access_tag, _Iter>
    {
    protected:
        typedef iterator_traits<_Iter> _traits;

    public:
        typedef typename _traits::value_type value_type;
        typedef typename _traits::pointer pointer;
        typedef value_type &reference;
        typedef typename _traits::difference_type difference_type;
        typedef typename iterator<random_access_tag, _Iter>::category category;

        pointer _pit;

        normal_iterator() : _pit() {};
        normal_iterator(pointer _loc_) : _pit(_loc_) {}

        reference operator[](difference_type _i_) const noexcept
        {
            return *(_pit + _i_);
        }

        normal_iterator &operator++()
        {
            _pit++;
            return *this;
        };

        reference operator*()
        {
            return *_pit;
        };

        bool operator!=(const normal_iterator &_other_)
        {
            return _pit != _other_._pit;
        }

        bool operator==(const normal_iterator &_other_)
        {
            return _pit == _other_._pit;
        }

        ~normal_iterator()
        {
            _pit = nullptr;
        };
    };

}
