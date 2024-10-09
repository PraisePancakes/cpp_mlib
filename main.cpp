#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"

int main()
{

  mlib::string s("hello");
  std::cout << s.size();
  std::cout << s;
  return 0;
}
