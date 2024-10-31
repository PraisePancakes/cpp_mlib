#include <iostream>
#include "lib/mstack.hpp"
#include "lib/algorithms/algorithms.hpp"
#include <stack>

int main()
{
  mlib::vec<int> v{1, 2, 3, 4, 5};

  mlib::reverse(v.begin(), v.end(), 0, 0);

  v.print();

  return 0;
};
