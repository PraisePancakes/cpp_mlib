#include <iostream>
#include "lib/mstack.hpp"
#include <stack>
int main()
{

  mlib::vec<int> v;
  for (size_t i = 0; i < 200; ++i)
  {
    v.push_back(i + 1);
  }

  v.print();

  return 0;
};
