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

            _dllnode_(value_type val) : _v_(val), _prev_(nullptr), _next_(nullptr) {};
            _dllnode_(value_type val, _dllnode_ *p, _dllnode_ *n) : _v_(val), _prev_(p), _next_(n) {};
            ~_dllnode_() {};
        };

        _dllnode_ *_head_;
        size_type _sz_;

    public:
        dllist() : _head_(nullptr), _sz_(0) {};
        dllist &operator=(const dllist &_other_) {};
        dllist(const dllist &_other_) {};
        dllist(dllist &&_other_) {
        };

        void append_back(const_reference v)
        {
            if (!_head_)
            {
                _head_ = new _dllnode_(v);
            }
            else
            {
                _dllnode_ *temp = _head_;
                _dllnode_ *p = nullptr;
                while (temp->_next_)
                {
                    p = temp;
                    temp = temp->_next_;
                }

                temp->_next_ = new _dllnode_(v, p, nullptr);
            }
            ++_sz_;
        };

        void append_front(const_reference v)
        {
            if (_head_ == nullptr)
            {
                _head_ = new _dllnode_(v);
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

            _dllnode_ *temp = _head_;

            while (temp->_next_->_next_)
            {
                temp = temp->_next_;
            }

            temp->_next_ = nullptr;
            _sz_--;
        };

        reference
        operator[](size_type _index_)
        {
            size_type counter = 0;
            _dllnode_ *temp = _head_;
            while (temp->_next_ && counter != _index_)
            {
                temp = temp->_next_;
                counter++;
            }

            return temp->_v_;
        };

        [[nodiscard]] size_type size() const noexcept
        {
            return _sz_;
        };

        ~dllist() {};
    };
};