#include <iostream>
#include "lib/mvector.hpp"
#include "tests/vector/test_vector.hpp"
#include <vector>

int main()
{
  test_vector();

  mlib::vec<int> v{0, 1, 2};

  for (mlib::vec<int>::iterator it = v.mbegin(); it != v.mend(); ++it)
  {
    std::cout << *it;
  }

  mlib::reverse_iterator<int> it;

  return 0;
};