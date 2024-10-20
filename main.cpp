#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"

int main()
{
  mlib::deque<int> d;

  for (size_t i = 0; i < 10; i++)
  {
    d.push_front(i);
  }

  for (size_t i = 0; i < d.size(); i++)
  {
    std::cout << d[i] << " : ";
  }

  return 0;
}
