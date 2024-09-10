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
class ITest
{
protected:
public:
    std::string _ITest_subject;
    __INPUT _ITest_input;
    __EXPOUTPUT _ITest_expected;
    TestID _ITest_id;
    bool _ITest_passed;
    std::function<void()> _ITest_operation;

    ITest(const std::string &__subj__, const TestID __id__, __INPUT __inp__, __EXPOUTPUT __expect__)
    {
        this->_ITest_subject = __subj__;
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

    virtual void set_operation(std::function<void()> __functor__) = 0;
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
    ~ITest() {};
};

// VecTest<mlib::vec<int>, mlib::vec<int>, mlib::vec<int>> vtest("PUSH_BACK", {0, 2, 3, 4}, {0, 2, 3, 4}, {0, 2, 3, 4, 5});
//

template <typename __INPUT, typename __EXPOUTPUT>
class VecTest : public ITest<__INPUT, __EXPOUTPUT>
{

public:
    VecTest(const std::string &__operation__, __INPUT __v_in__, __EXPOUTPUT __v_expect__) : ITest<__INPUT, __EXPOUTPUT>("[TEST][VECTOR][" + __operation__ + "]", TestID::TID_VECTOR, __v_in__, __v_expect__) {};

    void set_operation(std::function<void()> __functor__) override
    {
        std::cout << this->_ITest_subject;
        this->_ITest_operation = __functor__;
    };

    void execute()
    {
        if (this->_ITest_operation) // Check if operation is set
        {
            this->_ITest_operation();
            this->determine();
        }
    }
};