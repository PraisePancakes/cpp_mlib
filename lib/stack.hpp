#pragma once
#include "vector.hpp"

namespace mlib
{
    template <typename __STKTYPE>
    class stack
    {
        mlib::vec<__STKTYPE> _Stk_container;

    public:
        stack() {};

        // api
        void push(const __STKTYPE &__val__)
        {
            _Stk_container.push_back(__val__);
        };
        __STKTYPE &top() const
        {
            return _Stk_container[_Stk_container.size() - 1];
        }
        __STKTYPE &pop()
        {
            _Stk_container.pop_back();
        }
    };
}
