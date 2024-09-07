#include "lib/vector.hpp"
#include "tests/test_iterators.hpp"

int main()
{

  test_iterators();

  mlib::vec<int> v{0, 1, 2, 3, 4, 5};

  mlib::vec<int> vsliced = v.slice(1, 5);

  std::cout << "--------------- SLICED -----------------" << std::endl;
  for (size_t i = 0; i < vsliced.size(); i++)
  {
    std::cout << vsliced[i] << " : ";
  }
  return 0;
};