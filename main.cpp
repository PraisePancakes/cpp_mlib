#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{

  mlib::string s1("hello");
  mlib::string s3("hi");
  mlib::string s2 = s3 + s1;
  std::cout << s2;
  return 0;
}
