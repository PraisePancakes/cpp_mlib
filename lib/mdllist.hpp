#pragma once
#include "mallocator.hpp"

namespace mlib
{

    namespace internal
    {
        template <typename U>
        struct _dllnode_
        {
            _dllnode_ *_prev_;
            _dllnode_ *_next_;
            U _v_;

            _dllnode_() : _v_(), _prev_(nullptr), _next_(nullptr) {};
            _dllnode_(U val) : _v_(val), _prev_(nullptr), _next_(nullptr) {};
            _dllnode_(U val, _dllnode_ *p, _dllnode_ *n) : _v_(val), _prev_(p), _next_(n) {};
            ~_dllnode_() {};
        };
    };

    template <typename T, typename Alloc = mlib::allocator<internal::_dllnode_<T>>>
    class dllist
    {
        using allocator_traits = mlib::allocator_traits<Alloc>;
        Alloc m_allocator;

    public:
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using size_type = size_t;

    private:
        using this_type = dllist<T, Alloc>;

        internal::_dllnode_<T> *_head_;
        internal::_dllnode_<T> *_tail_;
        internal::_dllnode_<T> *_trail_;
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
            internal::_dllnode_<std::remove_const_t<U>> *m_iterator;

        public:
            impl_dllist_iterator() : m_iterator(nullptr) {
                                     };
            impl_dllist_iterator(internal::_dllnode_<std::remove_const_t<U>> *_loc_) : m_iterator(_loc_) {};

            impl_dllist_iterator(const internal::_dllnode_<std::remove_const_t<U>> *_loc_)
                : m_iterator(const_cast<internal::_dllnode_<std::remove_const_t<U>> *>(_loc_)) {};

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

        this_type &reverse()
        {
            internal::_dllnode_<T> *ahead = nullptr;
            internal::_dllnode_<T> *prev = nullptr;
            internal::_dllnode_<T> *curr = _head_;

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
            return *this;
        };

        this_type &append_back(const_reference v)
        {
            if (!_head_)
            {
                _head_ = m_allocator.allocate(1);
                m_allocator.construct(_head_, v);

                _trail_ = _head_;
                _head_->_next_ = _tail_;
            }
            else
            {
                _tail_ = m_allocator.allocate(1);
                m_allocator.construct(_tail_, v);
                _tail_->_prev_ = _trail_;
                _trail_->_next_ = _tail_;
                _tail_->_next_ = nullptr;
                _trail_ = _tail_;
                _tail_ = _tail_->_next_;
            }
            ++_sz_;
            return *this;
        };

        this_type &append_front(const_reference v)
        {
            if (_head_ == nullptr)
            {
                _head_ = m_allocator.allocate(1);
                m_allocator.construct(_head_, v);
                _trail_ = _head_;
                _head_->_next_ = _tail_;
            }
            else
            {
                internal::_dllnode_<T> *temp = m_allocator.allocate(1);
                m_allocator.construct(temp, v);

                temp->_next_ = _head_;
                _head_->_prev_ = temp;
                _head_ = _head_->_prev_;
            };
            _sz_++;
            return *this;
        };

        this_type &pop_front()
        {
            if (_head_ == nullptr)
                return *this;

            internal::_dllnode_<T> *temp = _head_;
            _head_ = _head_->_next_;
            m_allocator.destroy(temp);
            m_allocator.deallocate(temp, 1);

            _head_->_prev_ = nullptr;
            _sz_--;
            return *this;
        };

        this_type &pop_back()
        {
            if (_head_ == nullptr)
                return *this;

            _trail_->_next_ = nullptr;
            m_allocator.destroy(_tail_);
            m_allocator.deallocate(_tail_, 1);
            _tail_ = _trail_;
            _trail_ = _trail_->_prev_;
            _sz_--;
            return *this;
        };

        reference back() const noexcept
        {
            return _trail_->_v_;
        };

        Alloc &get_allocator() const noexcept
        {
            return this->m_allocator;
        };

        reference
        operator[](size_type _index_)
        {

            bool in_right_half = _locate_which_partition(_index_);

            if (in_right_half)
            {
                size_type counter = _sz_ - 1;
                internal::_dllnode_<T> *temp = _trail_;
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
                internal::_dllnode_<T> *temp = _head_;
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