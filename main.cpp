#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"
#include "lib/mdeque.hpp"

int main()
{
  mlib::vec<int> v({5, 3, 1, 2, 7});
  v.sort();

  v.print();
  return 0;
}
