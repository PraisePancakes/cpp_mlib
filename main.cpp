#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"

#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  mlib::vec<int> v1;

  v1.reserve(10);
  v1.push_back(10);
  std::cout << v1.capacity() << " : " << v1.size() << std::endl;
  v1.print();
  return 0;
}