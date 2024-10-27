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
    d.append_back(i);
  };

  d.reverse();

  for (size_t i = 0; i < d.size(); ++i)
  {
    std::cout << d[i] << " : ";
  }

  return 0;
};
