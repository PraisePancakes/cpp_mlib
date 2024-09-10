#include "lib/vector.hpp"
#include "tests/vector/iterators/test_iterators.hpp"
#include "tests/vector/methods/test_methods.hpp"
#include "lib/test.hpp"

int main()
{
  VecTest<mlib::vec<int>, mlib::vec<int>> v("PUSH_BACK", {0, 1, 2, 3}, {0, 1, 2, 3, 4});
  VecTest<mlib::vec<int>, mlib::vec<int>> v2("POP", {0, 1, 2, 3}, {0, 1, 2});

  v.set_operation([&v]()
                  { v._ITest_input.push_back(4); });
  v2.set_operation([&v2]()
                   { v2._ITest_input.pop_back(); });
  v.execute();
  v2.execute();
  std::cout << v.success();
  std::cout << v2.success();

  return 0;
};