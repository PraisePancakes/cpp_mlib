#include <iostream>
#include "lib/mvector.hpp"
#include "tests/vector/test_vector.hpp"
#include <vector>
#include <ctime>

int main()
{
  // test_vector();

  mlib::vec<int> v;
  std::vector<int> v1;

  std::clock_t begin = clock();
  for (size_t i = 0; i < 100000; i++)
  {
    v.push_back(1);
  }
  std::clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  std::cout << "mlib push_back time : " << elapsed_secs << std::endl;

  std::clock_t begin1 = clock();
  for (size_t i = 0; i < 100000; i++)
  {
    v1.push_back(1);
  }
  std::clock_t end1 = clock();
  double elapsed_secs1 = double(end1 - begin1) / CLOCKS_PER_SEC;
  std::cout << "std push_back time : " << elapsed_secs1 << std::endl;

  return 0;
};