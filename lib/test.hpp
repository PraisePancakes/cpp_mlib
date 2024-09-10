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
protected:
public:
    std::string _ITest_title;
    __INPUT _ITest_input;
    __EXPOUTPUT _ITest_expected;
    TestID _ITest_id;
    bool _ITest_passed;
    std::function<void()> _ITest_operation;

    MTest(const std::string &__title__, const TestID __id__, __INPUT __inp__, __EXPOUTPUT __expect__)
    {

        this->_ITest_title = "[TEST][" + __title__ + "]";
        this->_ITest_id = __id__;
        this->_ITest_input = __inp__;

        this->_ITest_expected = __expect__;
    };

    void fail()
    {
        this->_ITest_passed = false;
    }

    void pass()
    {
        this->_ITest_passed = true;
    }

    bool success() const
    {
        return _ITest_passed;
    }
    // pass lambda here

    void execute()
    {
        if (this->_ITest_operation) // Check if operation is set
        {
            this->_ITest_operation();
            this->determine();
        }
    }

        void set_operation(std::function<void()> __functor__)
    {
        std::cout << this->_ITest_title;
        this->_ITest_operation = __functor__;
    };
    void determine()
    {
        if (this->_ITest_expected == this->_ITest_input)
        {
            this->pass();
        }
        else
        {
            this->fail();
        };
    }
    ~MTest() {};
};
