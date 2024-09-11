#pragma once
#include <iostream>
#include "vector.hpp"
#include "stack.hpp"
#include <functional>
#include <any>

enum class TestID
{
    TID_ITERATOR,
    TID_VECTOR,
    TID_STACK,
};

template <typename __INPUT, typename __EXPOUTPUT>
class MTest
{
    bool _ITest_success() const
    {
        return _ITest_passed;
    }

    void _ITest_determine()
    {
        std::cout << this->_ITest_title << std::endl;
        if (this->_ITest_expected == this->_ITest_input)
        {

            this->_ITest_pass();
        }
        else
        {
            this->_ITest_fail();
        };
        std::cout << (_ITest_success() ? "passed" : "failed") << std::endl;
    }

    void _ITest_fail()
    {
        this->_ITest_passed = false;
    }

    void _ITest_pass()
    {
        this->_ITest_passed = true;
    }

    TestID _ITest_id;
    bool _ITest_passed;
    std::function<void()> _ITest_operation;
    std::string _ITest_title;

public:
    __INPUT _ITest_input;
    __EXPOUTPUT _ITest_expected;

    MTest(const std::string &__title__, const TestID __id__, __INPUT __inp__, __EXPOUTPUT __expect__)
    {

        this->_ITest_title = "[TEST][" + __title__ + "]";
        this->_ITest_id = __id__;
        this->_ITest_input = __inp__;

        this->_ITest_expected = __expect__;
    };

    // pass lambda here

    void execute()
    {
        if (this->_ITest_operation) // Check if operation is set
        {
            this->_ITest_operation();
            this->_ITest_determine();
        }
    }

    void set_operation(std::function<void()> __functor__)
    {
        this->_ITest_operation = __functor__;
    };

    ~MTest() {};
};
