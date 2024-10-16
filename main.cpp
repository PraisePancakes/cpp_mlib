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

  l.append_back(5);
  l.append_back(7);
  l.append_back(8);
  l.append_back(9);
  l.append_back(10);
  l.pop_back();
  l.append_back(20);
  l.append_back(50);
  l.pop_front();
  l.append_front(1);

  for (size_t i = 0; i < l.size(); i++)
  {
    std::cout << l[i] << std::endl;
  }
  return 0;
}
