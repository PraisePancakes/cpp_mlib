#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include "lib/mttraits.hpp"
#include <ctime>
#include <chrono>

enum E
{
};

class A
{
};

union U
{
};

int main()
{

  std::cout << mlib::is_enum<E>::value; // 1
  std::cout << mlib::is_enum<A>::value; // 0
  std::cout << mlib::is_union<U>::value;
  std::cout << mlib::is_union<A>::value;

  std::cout << mlib::is_function<int()>::value;
  std::cout << mlib::is_function<int>::value;
  // out 1010

  return 0;
}