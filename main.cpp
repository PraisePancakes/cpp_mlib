#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"

int main()
{
  mlib::vec<int> s;

  s.push_back(8);
  s.push_back(8);

  s.push_back(8);
  s.print();

  s.for_each([](int &x)
             { std::cout << (x = x + 4) << std::endl; });

  return 0;
}
