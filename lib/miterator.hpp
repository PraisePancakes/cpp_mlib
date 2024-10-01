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

    namespace _same_as_detail
    {
        template <typename T, typename U>
        concept _same_helper = std::is_same<T, U>::value;
    };

    template <typename T, typename U>
    concept same_as = _same_as_detail::_same_helper<T, U> && _same_as_detail::_same_helper<U, T>;

    template <typename Iterator, class = void>
    struct init_iterator_traits
    {
    };

    template <typename Iterator>
    struct init_iterator_traits<Iterator,
                                mlib::void_t<typename Iterator::iterator_category,
                                             typename Iterator::value_type,
                                             typename Iterator::difference_type,
                                             typename Iterator::pointer,
                                             typename Iterator::reference>>
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    template <typename Iter>
    struct iterator_traits : init_iterator_traits<Iter>
    {
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
        typedef const T *pointer;
        typedef const T &reference;
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;
    };

    template <typename I>
    concept Iterator = requires(I i) {
        { *i } -> mlib::same_as<typename mlib::iterator_traits<I>::reference>;
        { ++i } -> mlib::same_as<I &>;
        { i++ } -> mlib::same_as<I>;
    };

    template <class Iterator>
    struct normal_iterator
    {
    protected:
    public:
        typedef iterator_traits<Iterator> iter_traits;
        typedef random_access_iterator_tag iterator_category;
        iter_traits::pointer _pit;

        normal_iterator() : _pit() {};
        normal_iterator(iter_traits::pointer _loc_) : _pit(_loc_) {};
        normal_iterator(iter_traits::reference _loc_) : _pit(_loc_) {};

        iter_traits::reference operator[](iter_traits::difference_type _i_) const noexcept
        {
            return *(_pit + _i_);
        }

        normal_iterator &operator=(const normal_iterator<Iterator> &_other_)
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

    public:
        typedef iterator_traits<T> iter_traits;
        iter_traits::pointer _pit;

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

        reverse_iterator &operator=(const reverse_iterator<T> &_other_)
        {
            this->_pit = _other_._pit;
        };
    };

}
