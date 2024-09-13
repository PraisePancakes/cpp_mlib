#pragma once
#include <iostream>
#include "mvector.hpp"
#include "mstack.hpp"
#include <functional>
#include <any>

template <typename __INPUT, typename __EXPOUTPUT>
class MTest
{
    bool _M_passed = false;
    __EXPOUTPUT _M_expected;
    __INPUT _M_input;
    std::string _M_title;

public:
    MTest(__INPUT __in__, const std::string &__subj__)
    {
        this->_M_input = __in__;
        this->_M_title = "[TEST][" + __subj__ + "]";
        _M_passed = false;
    };

    bool test(std::function<__EXPOUTPUT()> __functor__, __EXPOUTPUT __exp_out__, const std::string &__desc__)
    {
        _M_expected = __exp_out__;
        __EXPOUTPUT result = __functor__();
        this->_M_title += "{" + __desc__ + "}";
        std::cout << this->_M_title << std::endl;
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

    __INPUT &input()
    {
        return this->_M_input;
    }

    ~MTest()
    {
        _M_input.~__INPUT();
        _M_expected.~__EXPOUTPUT();
    }
};
