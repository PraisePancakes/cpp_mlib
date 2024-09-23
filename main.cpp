#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{

  mlib::vec<int> v{1, 2, 3};

  for (auto i = v.mbegin(); i != v.mend(); ++i)
  {
    std::cout << *i;
  }

  return 0;
}