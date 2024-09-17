#pragma once

namespace mlib
{
    template <typename __CTy>
    class const_reverse_iterator
    {
        __CTy *_Iterator_ptr;

    public:
        const_reverse_iterator() : _Iterator_ptr(nullptr) {};
        const_reverse_iterator(__CTy *__iter_loc__) : _Iterator_ptr(__iter_loc__) {};
        const_reverse_iterator(const const_reverse_iterator &other) = delete;
        const_reverse_iterator(const_reverse_iterator &&other) = delete;

        bool operator!=(const const_reverse_iterator &other)
        {

            return _Iterator_ptr != other._Iterator_ptr;
        };

        const const_reverse_iterator &operator++()
        {
            --_Iterator_ptr;
            return *this;
        }

        const __CTy &operator*()
        {
            return *(_Iterator_ptr);
        };

        const const_reverse_iterator &operator--()
        {

            _Iterator_ptr++;
            return *this;
        };
        ~const_reverse_iterator() {};
    };

    template <typename __CTy>
    class iterator
    {
        __CTy *_Iterator_ptr;

    public:
        iterator() : _Iterator_ptr(nullptr) {}
        iterator(__CTy *__iter_loc__) : _Iterator_ptr(__iter_loc__) {}

        bool operator!=(const iterator &other)
        {

            return _Iterator_ptr != other._Iterator_ptr;
        };

        iterator &operator++()
        {

            ++_Iterator_ptr;
            return *this;
        }
        iterator &operator--()
        {

            --_Iterator_ptr;
            return *this;
        }

        __CTy &operator*() const
        {
            return *(_Iterator_ptr);
        };
        ~iterator() {};
    };

    template <typename __CTy>
    class reverse_iterator
    {
        __CTy *_Iterator_ptr;

    public:
        reverse_iterator() : _Iterator_ptr(nullptr) {};
        reverse_iterator(__CTy *__iter_loc__) : _Iterator_ptr(__iter_loc__) {};
        reverse_iterator(const reverse_iterator &other) = delete;
        reverse_iterator(reverse_iterator &&other) = delete;

        bool operator!=(const reverse_iterator &other)
        {

            return _Iterator_ptr != other._Iterator_ptr;
        };

        reverse_iterator &operator++()
        {
            --_Iterator_ptr;
            return *this;
        }

        __CTy &operator*()
        {
            return *(_Iterator_ptr);
        };

        reverse_iterator &operator--()
        {

            _Iterator_ptr++;
            return *this;
        }
    };

    template <typename __CTy>
    class const_iterator
    {
        __CTy *_Iterator_ptr;

    public:
        const_iterator() : _Iterator_ptr(nullptr) {}
        const_iterator(__CTy *__iter_loc__) : _Iterator_ptr(__iter_loc__) {}
        const_iterator(const const_iterator &other) = delete;
        const_iterator(const_iterator &&other) = delete;

        bool operator!=(const const_iterator &other)
        {

            return _Iterator_ptr != other._Iterator_ptr;
        };

        const const_iterator &operator++()
        {

            ++_Iterator_ptr;
            return *this;
        }

        const __CTy &operator*() const
        {
            return *(_Iterator_ptr);
        };
        ~const_iterator() {};
    };

}
