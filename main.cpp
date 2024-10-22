#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"
#include <iterator>

int main()
{
  mlib::vec<int> v{0, 1, 2, 3};

  for (auto it = v.rbegin(); it != v.rend(); ++it)
  {
    std::cout << *it;
  }
  return 0;
}
