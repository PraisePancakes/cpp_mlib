#include <iostream>
#include "lib/mstack.hpp"
#include "lib/algorithms/algorithms.hpp"
#include <stack>
int main()
{

  std::vector<int> v;

  for (size_t i = 0; i < 10; ++i)
  {
    v.push_back(i);
  }

  mlib::for_each(v.begin(), v.end(), [](int x)
                 { std::cout << x; });

  return 0;
};
