#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{
  mlib::vec<int> v{0, 1, 2, 3};

  v.print();
  return 0;
}
