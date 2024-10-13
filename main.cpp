#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"

/*TO DO FIX ITERATOR_TRAITS*/
int main()
{
  mlib::vec<int> s({1, 2, 3, 4, 5});

  mlib::vec<int> sp = s.slice(0, 2);
  std::cout << sp.size();
  sp.print();
  return 0;
}
