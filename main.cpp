#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{

  mlib::vec<int> v{1, 2, 3};
  mlib::vec<int> v1 = v.slice(0, 2);

  for (auto i = v1.mbegin(); i != v1.mend(); ++i)
  {
    std::cout << *i << std::endl;
  }

  std::cout << "fixed";

  return 0;
}