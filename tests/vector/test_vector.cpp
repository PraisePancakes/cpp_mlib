#include "test_vector.hpp"
#include "../../lib/vector.hpp"
#include "../../lib/test.hpp"

void test_push()
{
    MTest<mlib::vec<int>, mlib::vec<int>> v1("CASE 1 : VECTOR PUSH (4)", TestID::TID_VECTOR, {0, 1, 2, 3}, {0, 1, 2, 3, 4});
    v1.set_operation([&v1]()
                     { v1._ITest_input.push_back(4); });
    v1.execute();

    MTest<mlib::vec<int>, mlib::vec<int>> v2("CASE 2 : VECTOR PUSH (4, 5, 6)", TestID::TID_VECTOR, {0, 1, 2, 3}, {0, 1, 2, 3, 4, 5, 6});
    v2.set_operation([&v2]()
                     { v2._ITest_input.push_back(4);
                       v2._ITest_input.push_back(5);
                       v2._ITest_input.push_back(6); });
    v2.execute();
}

void test_pop()
{
    MTest<mlib::vec<int>, mlib::vec<int>> v1("CASE 1 : VECTOR POP x1", TestID::TID_VECTOR, {0, 1, 2, 3}, {0, 1, 2});
    v1.set_operation([&v1]()
                     { v1._ITest_input.pop_back(); });
    v1.execute();

    MTest<mlib::vec<int>, mlib::vec<int>> v2("CASE 2 : VECTOR POP x2 ", TestID::TID_VECTOR, {0, 1, 2, 3, 4, 5}, {0, 1, 2, 3});
    v2.set_operation([&v2]()
                     { v2._ITest_input.pop_back();
                       v2._ITest_input.pop_back(); });
    v2.execute();

    MTest<mlib::vec<int>, mlib::vec<int>> v3("CASE 3 : VECTOR POP SIZE = 0 ", TestID::TID_VECTOR, {}, {});
    v3.set_operation([&v3]()
                     { v3._ITest_input.pop_back();
                       v3._ITest_input.pop_back(); });
    v3.execute();
};

void test_vector()
{
    test_push();
    test_pop();
};
