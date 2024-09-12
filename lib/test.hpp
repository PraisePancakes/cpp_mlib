#pragma once
#include <iostream>
#include "vector.hpp"
#include "stack.hpp"
#include <functional>
#include <any>

template <typename __INPUT, typename __EXPOUTPUT>
class MTest
{
    bool _M_passed = false;
    __EXPOUTPUT _M_expected;
    __INPUT _M_input;

public:
    bool test(std::function<__EXPOUTPUT()> __functor__, __EXPOUTPUT __exp_out__)
    {
        _M_expected = __exp_out__;
        __EXPOUTPUT result = __functor__();
        if (result == _M_expected)
        {
            std::cout << "passed" << std::endl;
        }
        else
        {
            std::cout << "failed" << std::endl;
        }
        return result == _M_expected;
    }

    __INPUT& input()
    {
        return this->_M_input;
    }

    MTest(__INPUT __in__)
    {
        this->_M_input = __in__;

        _M_passed = false;
    };

    ~MTest()
    {
        _M_input.~__INPUT();
        _M_expected.~__EXPOUTPUT();
    }
};
