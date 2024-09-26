#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include "lib/mttraits.hpp"
#include <ctime>
#include <chrono>

class A
{
};

struct B
{
};

union C
{
};

int main()
{

  std::cout << mlib::is_class_v<A>;
  std::cout << mlib::is_class<B>::value;
  std::cout << mlib::is_class<C>::value;
  std::cout << mlib::is_ptr<int *>::value;
  std::cout << mlib::is_ptr<int>::value;

  // output 11010
  return 0;
}