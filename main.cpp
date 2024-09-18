#include <iostream>
#include "lib/mstring.hpp"
#include "tests/vector/test_vector.hpp"
#include <vector>
#include <ctime>
#include <chrono>

int main()
{
  // test_vector();
  mlib::string s("hi");
  mlib::string s1(s);

  std::cout << s1;
  return 0;
};