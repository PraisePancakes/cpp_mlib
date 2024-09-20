#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  mlib::vec<int> v;
  for (size_t i = 0; i < 5; i++)
  {
    v.push_back(1);
  }
  v.push_back({0, 1, 2});
  v.print();

  return 0;
}