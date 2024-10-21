#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"

int main()
{
  mlib::vec<int> v;

  v.push_back(5);

  v.insert(1, 6);
  v.insert(0, 4);
  std::cout << v[0];
  return 0;
}
