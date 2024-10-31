#include <iostream>
#include "lib/mstack.hpp"
#include "lib/algorithms/algorithms.hpp"
#include <stack>
#include <algorithm>

int main()
{
  mlib::vec<int> v{1, 3, 2, 8, 5, 3, 10, 7, 6};

  mlib::sort(v.rbegin(), v.rend());

  v.print();

  return 0;
};
