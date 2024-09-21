#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  mlib::vec<int> v{0, 1, 2};
  v.push_back(4);
  v.pop_back();
  v.push_back(5);
  v.print();
  return 0;
}