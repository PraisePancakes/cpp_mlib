#pragma once
#include "vector.hpp"

namespace mlib
{
    template <typename __STKTYPE>
    class stack
    {
        mlib::vec<__STKTYPE> _M_container;

    public:
        stack() {};

        // api
        void push(const __STKTYPE &__val__)
        {
            _M_container.push_back(__val__);
        };
        __STKTYPE &top() const
        {
            return _M_container[_M_container.size() - 1];
        }
        __STKTYPE &pop()
        {
            _M_container.pop_back();
        }
    };
}
