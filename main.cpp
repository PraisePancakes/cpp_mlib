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
    d.push_back(i + 1);
  }

  for (auto it = d.begin(); it != d.end(); it++)
  {
    std::cout << *it;
  };

  return 0;
};
