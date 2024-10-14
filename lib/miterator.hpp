#pragma once
#include <cstddef>
#include "mttraits.hpp"

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

    template <typename Iter>
    struct iterator_traits
    {
        typedef Iter::difference_type difference_type;
        typedef Iter::value_type value_type;
        typedef Iter::pointer pointer;
        typedef Iter::reference reference;
        typedef Iter::category category;
    };

    template <typename T>
    struct iterator_traits<T *> // int* , char* etc..
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;
    };

    template <typename T>
    struct iterator_traits<const T *> // int* , char* etc..
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef const T &const_reference;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;
    };

    template <class Iterator>
    struct normal_iterator
    {
    protected:
    public:
        typedef iterator_traits<Iterator> iter_traits;
        typedef random_access_iterator_tag iterator_category;
        Iterator _pit;

        normal_iterator() : _pit() {};
        normal_iterator(iter_traits::pointer _loc_) : _pit(_loc_) {};

        iter_traits::reference operator[](iter_traits::difference_type _i_) const noexcept
        {
            return *(_pit + _i_);
        }

        normal_iterator &operator=(const normal_iterator<Iterator> &_other_)
        {
            this->_pit = _other_._pit;
            return *this;
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

    template <typename Iterator>
    class reverse_iterator
    {

    public:
        typedef iterator_traits<Iterator> iter_traits;
        Iterator _pit;

        typedef typename iter_traits::difference_type difference_type;
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

        reverse_iterator &operator=(const reverse_iterator<Iterator> &_other_)
        {
            this->_pit = _other_._pit;
        };
    };

}
