#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"

#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  mlib::vec<int> v1{0, 1, 2, 3};

  for (auto it = v1.mbegin(); it != v1.mend(); ++it)
  {
    std::cout << *it;
  }
  std::cout << std::endl;
  for (auto it = v1.mrbegin(); it != v1.mrend(); ++it)
  {
    std::cout << *it;
  }

  return 0;
}