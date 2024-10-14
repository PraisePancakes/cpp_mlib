#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mdeque.hpp"
#include "tests/mvector/vector_push.cpp"

int main()
{
  if (TEST_VECTOR())
  {
    std::cout << "vec::push_back passed all cases" << std::endl;
  }
  else
  {
    std::cout << "vec::push_back failed a case" << std::endl;
  };

  return 0;
}
