#pragma once
#include "mvector.hpp"

namespace mlib
{
    template <typename _CTy>
    class stack
    {
        mlib::vec<_CTy> _M_container;

    public:
        stack() {};

        // api

        size_t size() const
        {
            return _M_container.size();
        }

        void push(const _CTy &__val__)
        {
            _M_container.push_back(__val__);
        };
        _CTy &top() const
        {
            if (size() == 0)
            {
                return _M_container.at(0);
            };
            return _M_container[size() - 1];
        }
        void pop()
        {
            if (_M_container.empty())
            {
                return;
            }
            _M_container.pop_back();
        }
    };
}
