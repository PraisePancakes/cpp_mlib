#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mdllist.hpp"
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

  mlib::dllist<int> l;

  l.append(5);
  l.append(7);
  std::cout << l.size() << std::endl;

  for (size_t i = 0; i < l.size(); i++)
  {
    std::cout << l[i];
  }
  return 0;
}
