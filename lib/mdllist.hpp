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

    public:
        dllist() : _head_(nullptr), _tail_(nullptr), _trail_(nullptr), _sz_(0) {};
        dllist &operator=(const dllist &_other_) {};
        dllist(const dllist &_other_) {};
        dllist(dllist &&_other_) {
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