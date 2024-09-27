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

struct D : public B
{
};

int main()
{

  std::cout << mlib::is_class_v<A>;
  std::cout << mlib::is_class<B>::value;
  std::cout << mlib::is_class<C>::value;
  std::cout << mlib::is_ptr<int *>::value;
  std::cout << mlib::is_ptr<int>::value;

  typedef mlib::integral_constant<int, 5> iconst;
  iconst c;

  std::cout << c;
  std::cout << c();
  std::cout << iconst::value;
  std::cout << mlib::is_base_of<B, D>::value;
  std::cout << mlib::is_base_of<B, A>::value;
  // output 1101055510
  return 0;
}