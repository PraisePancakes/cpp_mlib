#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{

  mlib::string v("hi");

  mlib::string::iterator it;
  // or mlib::vec<char>::iterator it;

  for (it = v.begin(); it != v.end(); ++it)
  {
    std::cout << *it << std::endl;
  }

  return 0;
}
