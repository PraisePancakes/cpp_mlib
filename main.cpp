#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"
#include <iterator>
#include "lib/mstring.hpp"

int main()
{
  mlib::string str = "hello world";

  for (auto it = str.crbegin(); it != str.crend(); it++)
  {

    std::cout << *it;
  }

  return 0;
};
