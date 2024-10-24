#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"
#include <iterator>
#include "lib/mstring.hpp"

int main()
{
  mlib::deque<int> d;

  for (size_t i = 0; i < 5; i++)
  {
    d.push_back(i);
  }

  for (auto it = d.cbegin(); it != d.cend(); it++)
  {
    std::cout << *it;
  }

  return 0;
};
