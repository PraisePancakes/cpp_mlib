#pragma once
#include <cstddef>

namespace mlib
{

    struct input_iterator_tag
    {
    };

    struct output_iterator_tag
    {
    };

    struct forward_iterator_tag : public input_iterator_tag
    {
    };

    struct bidirectional_tag : public forward_iterator_tag
    {
    };

    struct random_access_iterator_tag
    {
    };

    struct contiguous_iterator_tag : public random_access_iterator_tag
    {
    };

    template <typename I>
    concept in_or_out_iterator = requires(I i) {
        { *i };
    };

    template <typename T, typename _Cat>
    struct iterator_traits
    {
        typedef _Cat category;
        typedef T value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;
    };

    template <typename T, typename _Cat>
    struct iterator_traits<T *, _Cat> // int* , char* etc..
    {
        typedef _Cat category;
        typedef T value_type;
        typedef value_type *pointer;
        typedef const pointer const_pointer;
        typedef value_type &reference;
        typedef const reference &const_reference;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;
    };

    template <class T>
    struct normal_iterator
    {
    protected:
        typedef iterator_traits<T, random_access_iterator_tag> iter_traits;

    public:
        iter_traits::pointer _pit;

        normal_iterator() : _pit() {};
        normal_iterator(iter_traits::pointer _loc_) : _pit(_loc_) {};
        normal_iterator(iter_traits::const_reference _loc_) : _pit(_loc_) {};

        iter_traits::reference operator[](iter_traits::difference_type _i_) const noexcept
        {
            return *(_pit + _i_);
        }

        normal_iterator &operator=(const normal_iterator<T> &_other_)
        {
            this->_pit = _other_._pit;
        };

        normal_iterator &operator++()
        {
            _pit++;
            return *this;
        };

        iter_traits::reference operator*()
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

    template <typename T>
    class reverse_iterator
    {
        typedef iterator_traits<T, bidirectional_tag> iter_traits;

    protected:
        typedef typename iter_traits::difference_type difference_type;

    public:
        iter_traits::pointer _pit;

        reverse_iterator() : _pit() {};
        reverse_iterator(iter_traits::pointer _loc_) : _pit(_loc_) {};

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

        iter_traits::reference operator*()
        {
            return *this->_pit;
        }

        reverse_iterator &operator=(const reverse_iterator<T> &_other_)
        {
            this->_pit = _other_._pit;
        };
    };

}
