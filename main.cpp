#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  mlib::vec<int> v{0, 1, 2, 3};

  mlib::vec<int> v1 = v.slice(0, 2);

  v1.print();
  return 0;
}