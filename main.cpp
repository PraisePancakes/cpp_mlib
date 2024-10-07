#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"

int main()
{

  mlib::vec<int> v2(mlib::vec<int>{0, 1, 2, 3});
  mlib::vec<int> v(std::move(v2));

  v.print();

  mlib::mtester t;

  t.push_operation<int, int>(2, [](int x) -> bool
                             { return x == 2; }, true);

  t.push_operation<mlib::vec<int>, int>({0, 2, 2}, [](mlib::vec<int> x) -> int
                                        { return x[1] + x[2]; }, 4);

  t.run_tests();

  return 0;
}
