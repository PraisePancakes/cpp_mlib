#pragma once
#include <iostream>
#include "vector.hpp"
#include "stack.hpp"
#include <functional>
#include <any>

template <typename __INPUT, typename __EXPOUTPUT>
class MTest
{

public:
    __INPUT _MTEST_input;
    __EXPOUTPUT _MTEST_expected;
    MTest(__INPUT __in__, __EXPOUTPUT __ein__)
    {
        this->_MTEST_input = __in__;
        this->_MTEST_expected = __ein__;
    };

    bool test(std::function<__EXPOUTPUT()> __functor__)
    {
        __EXPOUTPUT result = __functor__();
        if (result == _MTEST_expected)
        {
            std::cout << "passed" << std::endl;
        }
        else
        {
            std::cout << "failed" << std::endl;
        }
        return result == _MTEST_expected;
    }
};
