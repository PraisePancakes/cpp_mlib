#include "../../lib/mvector.hpp"
#include "../../lib/mtest.hpp"

bool TEST_VECTOR()
{
    mlib::mtester tester;

    tester.push_operation<mlib::vec<int>, mlib::vec<int>>({0, 1, 2, 3}, [](mlib::vec<int> x) -> mlib::vec<int>
                                                          { 
                                                              x.push_back(4); 
                                                              return x; }, {0, 1, 2, 3, 4});

    tester.push_operation<mlib::vec<int>, mlib::vec<int>>({0, 1, 2, 3}, [](mlib::vec<int> x) -> mlib::vec<int>
                                                          { x.push_back({4, 5, 6, 7});
                                                                        return x; }, {0, 1, 2, 3, 4, 5, 6, 7});

    return tester.run_tests();
};
