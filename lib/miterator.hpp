#pragma once
#include <cstddef>
#include "mttraits.hpp"

namespace mlib
{

    struct input_iterator_tag
    {
    };

    struct forward_iterator_tag : public input_iterator_tag // iterators that may only move forward i.e singly-linked list,
    {
    };

    struct bidirectional_tag : public forward_iterator_tag // bidirectional iterators are a type of forward iterators that may also move backwards, i.e doubly-linked list
    {
    };

    struct output_iterator_tag
    {
    };

    struct random_access_iterator_tag // random_access iterators may be used on containers that allow truely random access via 'operator[](const size_t index)' i.e. vectors / arrays / deque / queue
    {
    };

    /*
        contiguous iterators are also random access however,
        they must perform on containers that promise a contiguous memory layout, i.e vectors / arrays.
        Though they may be used depending on container implementation, they are forbidden to be used in mlib::deque and mlib::queue,
        as these containers do not promise contiguous memory (implemented via memory chunks.)
    */

    struct contiguous_iterator_tag : public random_access_iterator_tag
    {
    };

    /*
        @brief
        Iterator traits represents an interface to which you may forward custom iterators to allowing for type extraction.

    */

    template <typename Iter>
    struct iterator_traits
    {
        typedef Iter::difference_type difference_type;
        typedef Iter::value_type value_type;
        typedef Iter::pointer pointer;
        typedef Iter::const_pointer const_pointer;
        typedef Iter::const_reference const_reference;
        typedef Iter::reference reference;
        typedef Iter::category category;
    };

    /*
        @brief
            template specializations for iterator_traits :
        @param T* / const T*
            T* and const T* will specialize for pointers specifically,
            in a quasi-SFINAE way we can use raw pointers as iterators as well as user defined iterators.
            no value types can be used as an iterator (int, char, etc...)
    */

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

    template <typename Iter>
    class reverse_iterator
    {
        typedef typename iterator_traits<Iter>::category category;
        typedef typename iterator_traits<Iter>::value_type value_type;
        typedef typename iterator_traits<Iter>::pointer pointer;
        typedef typename iterator_traits<Iter>::const_pointer const_pointer;
        typedef typename iterator_traits<Iter>::reference reference;
        typedef typename iterator_traits<Iter>::const_reference const_reference;
        Iter m_RIterator;

    public:
        reverse_iterator() : m_RIterator(nullptr) {};
        reverse_iterator(Iter _loc_) : m_RIterator(_loc_) {};
        reverse_iterator &operator++()
        {
            m_RIterator--;
            return *this;
        };

        reverse_iterator &operator--()
        {
            m_RIterator++;
            return *this;
        };

        reverse_iterator operator++(int)
        {
            reverse_iterator temp = *this;
            --(m_RIterator);
            return *this;
        }

        reference operator*()
        {
            return *m_RIterator;
        };

        bool operator==(const reverse_iterator &other)
        {
            return this->m_RIterator == other.m_RIterator;
        };

        bool operator!=(const reverse_iterator &other)
        {
            return this->m_RIterator != other.m_RIterator;
        };
        ~reverse_iterator() = default;
    };

}
