#pragma once
#include <cstddef>

namespace mlib
{
    struct random_access_tag
    {
    };

    struct bidirectional_tag
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

    template <class _ITCategory, class _Ty, class _ITDistance = std::ptrdiff_t, class _ITPointer = _Ty *, class _ITConstPointer = const _Ty *>
    struct iterator
    {

        typedef _Ty value_type;
        typedef _ITCategory category;
        typedef _ITDistance difference_type;
        typedef _ITPointer pointer;
        typedef _ITConstPointer const_pointer;
    };

    template <class _Ty>
    struct normal_iterator : public iterator<random_access_tag, _Ty>
    {
    protected:
        typedef iterator_traits<_Ty> _traits;

    public:
        typedef typename _traits::value_type value_type;
        typedef typename _traits::pointer pointer;
        typedef value_type &reference;
        typedef typename _traits::difference_type difference_type;
        typedef typename iterator<random_access_tag, _Ty>::category category;

        pointer _pit;

        normal_iterator() : _pit() {};
        normal_iterator(pointer _loc_) : _pit(_loc_) {}

        reference operator[](difference_type _i_) const noexcept
        {
            return *(_pit + _i_);
        }

        normal_iterator &operator=(const normal_iterator<_Ty> &_other_)
        {
            this->_pit = _other_._pit;
        };

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

    template <typename _Ty>
    class reverse_iterator : public iterator<bidirectional_tag, _Ty>
    {
        typedef iterator_traits<_Ty> _traits;

    protected:
        typedef typename _traits::value_type value_type;
        typedef typename _traits::pointer pointer;
        typedef value_type &reference;
        typedef typename _traits::difference_type difference_type;
        typedef typename iterator<random_access_tag, _Ty>::category category;

    public:
        pointer _pit;

        reverse_iterator() : _pit() {};
        reverse_iterator(pointer _loc_) : _pit(_loc_) {};

        reverse_iterator &operator++()
        {
            this->_pit--;
            return *this;
        };

        bool operator==(const reverse_iterator &_other_)
        {
            return this->_pit == _other_._pit;
        }

        bool operator!=(const reverse_iterator &_other_)
        {
            return this->_pit != _other_._pit;
        }

        reference operator*()
        {
            return *this->_pit;
        }

        reverse_iterator &operator=(const reverse_iterator<_Ty> &_other_)
        {
            this->_pit = _other_._pit;
        };
    };

}
