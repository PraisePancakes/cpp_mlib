#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mdllist.hpp"
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

  mlib::deque<int> d;

  d.push_back(1);
  d.push_back(2);
  d.push_back(3);

  for (size_t i = 0; i < 3; i++)
  {
    std::cout << d[i];
  }

  return 0;
}
