#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{
  mlib::vec<int> s(15, 5);

  std::cout << s.size() << std::endl;
  s.print();

  return 0;
}
