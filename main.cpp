#include "lib/vector.hpp"
#include "tests/vector/iterators/test_iterators.hpp"
#include "tests/vector/methods/test_methods.hpp"
#include "lib/test.hpp"

int main()
{

  // test on the spot

  MTest<mlib::vec<int>, mlib::vec<int>> v2("VECTOR POP", TestID::TID_VECTOR, {0, 1, 2, 3}, {0, 1, 2});

  v2.set_operation([&v2]()
                   { v2._ITest_input.pop_back(); });

  v2.execute();

  std::cout << v2.success() << std::endl;

  return 0;
};