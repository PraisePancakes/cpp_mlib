#include "mvector.hpp"

namespace mlib
{
    struct operation_base
    {
        virtual bool test_operation() = 0;
    };

    template <typename In, typename ExOut>
    struct operation : public operation_base
    {
        In input;
        std::function<ExOut(In)> op;
        ExOut exoutput;

        operation(In _input_value_, std::function<ExOut(In)> oper, ExOut _expected_output_val_) : input(_input_value_), op(oper), exoutput(_expected_output_val_) {

                                                                                                  };
        bool test_operation() override
        {
            if (op(input) == exoutput)
            {
                std::cout << "Passed Test Case" << std::endl;
                return true;
            };

            std::cout << "Failed Test Case" << std::endl;
            return false;
        };

        ~operation() {};
    };

    class mtester
    {
        // vector of operations to run, each operation will have an input and output

        mlib::vec<operation_base *> cases;

    public:
        mtester() {};

        template <typename In, typename ExOut>
        bool test_operation(In _input_value_, std::function<ExOut(In)> op, ExOut _expected_output_val_)
        {
            if (op(_input_value_) == _expected_output_val_)
            {
                std::cout << "Passed Test Case" << std::endl;
                return true;
            };

            std::cout << "Failed Test Case" << std::endl;
            return false;
        };

        template <typename In, typename ExOut>
        void push_operation(const operation<In, ExOut> &op)
        {
            cases.push_back(op);
        };

        template <typename In, typename ExOut>
        void push_operation(In _input_value_, std::function<ExOut(In)> op, ExOut _expected_output_val_)
        {
            operation_base *to_do = new operation(_input_value_, op, _expected_output_val_);
            cases.push_back(to_do);
        };

        bool run_tests() const
        {
            for (size_t i = 0; i < cases.size(); i++)
            {
                if (!cases[i]->test_operation())
                    return false;
            }

            return true;
        };

        ~mtester() {

        };
    };
};