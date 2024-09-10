#pragma once
#include "vector.hpp"

namespace mlib
{
    template <typename T>
    class stack
    {
        mlib::vec<T> _Stk_container;

    public:
        stack() {};

        // api
        void push(const T &__val__)
        {
            _Stk_container.push_back(__val__);
        };
        T &top() const
        {
            return _Stk_container[_Stk_container.size() - 1];
        }
        T &pop()
        {
            _Stk_container.pop_back();
        }
    };
}
