#include <iostream>
#include "lib/mstack.hpp"
#include "lib/algorithms/algorithms.hpp"
#include <stack>
int main()
{

  mlib::vec<int> v;

  for (size_t i = 0; i < 10; ++i)
  {
    v.push_back(i);
  }

  mlib::reverse(v.begin(), v.end());

  v.print();

  return 0;
};
