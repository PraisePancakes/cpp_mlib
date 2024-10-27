#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"
#include <iterator>
#include "lib/mstring.hpp"
#include "lib/mdllist.hpp"

int main()
{
  mlib::dllist<int> d;

  for (int i = 0; i < 10; ++i)
  {
    d.append_back(i + 1);
  };

  for (auto it = d.begin(); it != d.end(); ++it)
  {
    std::cout << *it;
  }

  return 0;
};
