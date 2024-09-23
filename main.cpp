#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{

  mlib::vec<int> v{0, 1, 2, 3, 4, 5};

  v.splice(2, 2, {1, 2});
  v.print();

  return 0;
}