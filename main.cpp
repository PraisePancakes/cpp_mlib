#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"
#include "lib/mdeque.hpp"

int main()
{
  mlib::vec<int> v({0, 1, 2, 3, 4, 5});
  v.splice(0, 2, {7, 8});

  v.print();
  return 0;
}
