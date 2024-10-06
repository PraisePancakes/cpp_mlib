#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{

  mlib::string s1("hellooooooooo");

  std::cout << s1.size();
  std::cout << s1;
  return 0;
}
