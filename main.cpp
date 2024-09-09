#include "lib/vector.hpp"
#include "tests/test_iterators.hpp"

int main()
{

  // test_iterators();

  mlib::vec<char> v{'a', 'b', 'c', 'd', 'e', 'f'};

  std::cout << "--------------- splice -----------------" << std::endl;

  for (size_t i = 0; i < v.size(); i++)
  {
    std::cout << v[i] << " : ";
  }
  std::cout << std::endl;
  v.splice(2, 2, {'r', 'z'});
  for (size_t i = 0; i < v.size(); i++)
  {
    std::cout << v[i] << " : ";
  }
  return 0;
};