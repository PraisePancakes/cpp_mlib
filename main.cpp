#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  mlib::vec<int> v;
  v.push_back({3, 4, 5});
  v.print();
  return 0;
}