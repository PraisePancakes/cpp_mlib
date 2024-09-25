#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"

#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  mlib::vec<int> v1{0, 1, 2, 3, 4, 5};

  v1.push_back(3);
  v1.push_back(4);
  v1.push_back(5);
  v1.push_back(6);
  v1.push_back(6);
  v1.push_back(7);
  v1.push_back(8);

  std::cout << v1.size() << " : " << v1.capacity() << std::endl;

  v1.print();

  return 0;
}