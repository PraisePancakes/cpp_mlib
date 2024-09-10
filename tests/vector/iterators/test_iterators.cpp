#include "test_iterators.hpp"

void test_regular_iterator(const mlib::vec<int> &v)
{
  std::cout << "-------------" << std::endl;
  std::cout << "regular iterator (elem => elem + 1)" << std::endl;

  for (auto it = v.mbegin(); it != v.mend(); ++it)
  {
    *it = *it + 1;
    std::cout << *it << " , ";
  }
  std::cout << std::endl;
};
void test_reverse_iterator(const mlib::vec<int> &v)
{
  std::cout << "-------------" << std::endl;
  std::cout << "reverse iterator (elem => elem + 1)" << std::endl;

  for (auto it = v.mrbegin(); it != v.mrend(); ++it)
  {
    *it = *it + 1;
    std::cout << *it << " , ";
  }
  std::cout << std::endl;
};
void test_const_iterator(const mlib::vec<int> &v)
{
  std::cout << "-------------" << std::endl;
  std::cout << "const iterator" << std::endl;

  for (auto it = v.mcbegin(); it != v.mcend(); ++it)
  {
    std::cout << *it << " , ";
    //*it = 4; cannot assign because is const
  }
  std::cout << std::endl;
};
void test_const_reverse_iterator(const mlib::vec<int> &v)
{
  std::cout << "-------------" << std::endl;
  std::cout << "const reverse iterator" << std::endl;

  for (auto it = v.mcrbegin(); it != v.mcrend(); ++it)
  {
    std::cout << *it << " , ";
    //*it = 4; cannot assign because is const
  }
  std::cout << std::endl;
};

void test_iterators()
{
  mlib::vec<int> v1{0, 1, 2, 3};
  std::cout << "[TEST][VECTOR][ITERATORS]" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << "subject : ";
  for (size_t i = 0; i < v1.size(); i++)
  {
    std::cout << v1[i] << " , ";
  }
  std::cout << std::endl;

  test_regular_iterator(v1);
  test_reverse_iterator(v1);
  test_const_iterator(v1);
  test_const_reverse_iterator(v1);
}