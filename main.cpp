#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{
  mlib::string s = "hello";

  mlib::normal_iterator<char> it;

  for (it = s.begin(); it != s.end(); ++it)
  {
    std::cout << *it;
  };
  return 0;
}
