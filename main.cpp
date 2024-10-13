#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"

/*TO DO FIX ITERATOR_TRAITS*/
int main()
{
  mlib::vec<int> s({0, 1, 2, 3});

  s.splice(0, 1, {1, 2, 3});

  s.print();
  return 0;
}
