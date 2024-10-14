#include "../../lib/mvector.hpp"
#include "../../lib/mtest.hpp"

bool TEST_VECTOR()
{
    bool passed_all = false;
    mlib::mtester tester;

    // The lambda should take an int argument and return an int.
    tester.push_operation<mlib::vec<int>, mlib::vec<int>>({0, 1, 2, 3}, [](mlib::vec<int> x) -> mlib::vec<int>
                                                          { 
                                                              x.push_back(4); 
                                                              return x; }, {0, 1, 2, 3, 4});

    tester.push_operation<mlib::vec<int>, mlib::vec<int>>({0, 1, 2, 3}, [](mlib::vec<int> x) -> mlib::vec<int>
                                                          { x.push_back({4, 5, 6, 7});
                                                                        return x; }, {0, 1, 2, 3, 4, 5, 6, 7});
    tester.run_tests();
    return passed_all;
};
