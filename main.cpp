#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include "lib/mttraits.hpp"
#include <ctime>
#include <chrono>

int main()
{

  std::cout << mlib::is_void<int>::value;
  return 0;
}