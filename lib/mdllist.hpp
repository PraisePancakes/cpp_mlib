#pragma once
#include "mallocator.hpp"

namespace mlib
{
    template <typename T, typename Alloc = mlib::allocator<T>>
    class dllist
    {
        using allocator_traits = mlib::allocator_traits<Alloc>;

    public:
        using value_type = allocator_traits::value_type;
        using reference = allocator_traits::reference;
        using const_reference = allocator_traits::const_reference;
        using size_type = allocator_traits::size_type;

    private:
        struct _dllnode_
        {
            _dllnode_ *_prev_;
            _dllnode_ *_next_;
            value_type _v_;

            _dllnode_() : _v_(), _prev_(nullptr), _next_(nullptr) {};
            _dllnode_(value_type val) : _v_(val), _prev_(nullptr), _next_(nullptr) {};
            _dllnode_(value_type val, _dllnode_ *p, _dllnode_ *n) : _v_(val), _prev_(p), _next_(n) {};
            ~_dllnode_() {};
        };

        _dllnode_ *_head_;
        _dllnode_ *_tail_;
        _dllnode_ *_trail_;
        size_type _sz_;

        bool _locate_which_partition(size_t _index_)
        {
            return (_index_ >= (std::floor(_sz_ / 2)));
        };

        template <typename U>
        class impl_dllist_iterator
        {
        public:
            using value_type = U;
            using pointer = U *;
            using const_pointer = const U *;
            using reference = U &;
            using const_reference = const U &;
            using category = bidirectional_tag;
            using size_type = size_t;
            using difference_type = ptrdiff_t;

        private:
            _dllnode_ *m_iterator;

        public:
            impl_dllist_iterator() : m_iterator(nullptr) {
                                     };
            impl_dllist_iterator(_dllnode_ *_loc_) : m_iterator(_loc_) {};

            reference operator*()
            {
                return m_iterator->_v_;
            };

            impl_dllist_iterator &operator++()
            {
                m_iterator = m_iterator->_next_;
                return *this;
            };

            impl_dllist_iterator &operator--()
            {
                m_iterator = m_iterator->_prev_;
                return *this;
            };

            impl_dllist_iterator operator--(int)
            {
                impl_dllist_iterator temp = *this;
                --(*this);
                return *this;
            };

            impl_dllist_iterator operator++(int)
            {
                impl_dllist_iterator temp = *this;
                ++(*this);
                return *this;
            };

            bool operator==(const impl_dllist_iterator &_other_) const noexcept
            {
                return this->m_iterator == _other_.m_iterator;
            };

            bool operator!=(const impl_dllist_iterator &_other_) const noexcept
            {
                return !(*this == _other_);
            };

            ~impl_dllist_iterator() {};
        };

    public:
        using iterator = impl_dllist_iterator<T>;
        using const_iterator = impl_dllist_iterator<const T>;
        using reverse_iterator = mlib::reverse_iterator<iterator>;
        using const_reverse_iterator = mlib::reverse_iterator<const_iterator>;

        dllist() : _head_(nullptr), _tail_(nullptr), _trail_(nullptr), _sz_(0) {};
        dllist &operator=(const dllist &_other_) {};
        dllist(const dllist &_other_) {};
        dllist(dllist &&_other_) {
        };

        iterator begin() const
        {
            return iterator(this->_head_);
        };

        iterator end() const
        {
            return iterator(this->_tail_);
        };
        const_iterator cbegin() const
        {
            return const_iterator(this->_head_);
        };

        const_iterator cend() const
        {
            return const_iterator(this->_tail_);
        };

        reverse_iterator rbegin() const
        {
            return reverse_iterator(this->_trail_);
        };

        reverse_iterator rend() const
        {
            return reverse_iterator(this->_head_->_prev_);
        };

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(this->_trail_);
        };

        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(this->_head_->_prev_);
        };

        void reverse()
        {
            _dllnode_ *ahead = nullptr;
            _dllnode_ *prev = nullptr;
            _dllnode_ *curr = _head_;

            while (curr)
            {
                ahead = curr->_next_;
                curr->_next_ = prev;
                curr->_prev_ = ahead;
                prev = curr;
                curr = ahead;
            }

            _trail_ = _head_;
            _head_ = prev;
        };

        void append_back(const_reference v)
        {
            if (!_head_)
            {
                _head_ = new _dllnode_(v);
                _trail_ = _head_;
                _head_->_next_ = _tail_;
            }
            else
            {
                _tail_ = new _dllnode_(v);
                _tail_->_prev_ = _trail_;
                _trail_->_next_ = _tail_;
                _tail_->_next_ = nullptr;
                _trail_ = _tail_;
                _tail_ = _tail_->_next_;
            }
            ++_sz_;
        };

        void append_front(const_reference v)
        {
            if (_head_ == nullptr)
            {
                _head_ = new _dllnode_(v);
                _trail_ = _head_;
                _head_->_next_ = _tail_;
            }
            else
            {
                _dllnode_ *temp = new _dllnode_(v);
                temp->_next_ = _head_;
                _head_->_prev_ = temp;
                _head_ = _head_->_prev_;
            };
            _sz_++;
        };

        void pop_front()
        {
            if (_head_ == nullptr)
                return;

            _dllnode_ *temp = _head_;
            _head_ = _head_->_next_;
            delete temp;
            _head_->_prev_ = nullptr;
            _sz_--;
        };

        void pop_back()
        {
            if (_head_ == nullptr)
                return;

            _trail_->_next_ = nullptr;
            _tail_ = _trail_;
            _trail_ = _trail_->_prev_;
            _sz_--;
        };

        reference
        operator[](size_type _index_)
        {

            bool in_right_half = _locate_which_partition(_index_);

            if (in_right_half)
            {
                size_type counter = _sz_ - 1;
                _dllnode_ *temp = _trail_;
                while (temp->_prev_ && counter != _index_)
                {
                    temp = temp->_prev_;
                    counter--;
                }

                return temp->_v_;
            }
            else
            {
                size_type counter = 0;
                _dllnode_ *temp = _head_;
                while (temp->_next_ && counter != _index_)
                {
                    temp = temp->_next_;
                    counter++;
                }

                return temp->_v_;
            }
        };

        [[nodiscard]] size_type size() const noexcept
        {
            return _sz_;
        };

        ~dllist() {};
    };
};