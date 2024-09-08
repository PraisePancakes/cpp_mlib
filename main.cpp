#include "lib/vector.hpp"
#include "tests/test_iterators.hpp"

int main()
{

  // test_iterators();

  mlib::vec<int> v{0, 1, 2, 3, 4, 5};

  v.splice(0, 3, {9, 2, 3});

  std::cout << "--------------- splice -----------------" << std::endl;
  for (size_t i = 0; i < v.size(); i++)
  {
    std::cout << v[i] << " : ";
  }
  return 0;
};