#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{
  mlib::string s = "hello";
  mlib::string s1(s);



  std::cout << s1;

  return 0;
}
