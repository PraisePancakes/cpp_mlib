#include "test_methods.hpp"

void test_push()
{
    std::cout << "[TEST][METHOD][PUSH] " << std::endl;
    mlib::vec<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    for (auto it = v.mbegin(); it != v.mend(); ++it)
    {
        std::cout << *it << " : ";
    };

    std::cout << std::endl;
}

void test_methods()
{
    test_push();
};