#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{

  mlib::string s1("this is not sso");
  mlib::string s2("sso");

  std::cout << s1.size() << std::endl;
  std::cout << s2.size() << std::endl;
  std::cout << s1.size() + s2.size() << std::endl;

  s1 = s1 + s2;

  std::cout << s1.size() << std::endl;

  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl;

  return 0;
}
