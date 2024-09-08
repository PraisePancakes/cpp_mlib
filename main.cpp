#include "lib/vector.hpp"
#include "tests/test_iterators.hpp"

int main()
{

  // test_iterators();

  mlib::vec<int> v{0, 4, 5, 6, 7, 8};

  v.splice(0, 3, {1, 2, 3, 4});

  std::cout << "--------------- splice -----------------" << std::endl;
  for (size_t i = 0; i < v.size(); i++)
  {
    std::cout << v[i] << " : ";
  }
  return 0;
};