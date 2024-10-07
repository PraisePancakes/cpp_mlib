#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"

int main()
{
  mlib::mtester t;

  t.push_operation<int, int>(2, [](int x) -> bool
                             { return x == 2; }, true);
  t.push_operation<char, int>('a', [](char x) -> bool
                              { return x == 'v'; }, true);

  t.run_tests();

  return 0;
}
