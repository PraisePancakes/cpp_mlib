#include "lib/mvector.hpp"
#include "lib/mtest.hpp"
#include "tests/vector/test_vector.hpp"
#include "lib/mstring.hpp"

int main()
{

  // test_vector();

  mlib::string s("hello");

  for (size_t i = 0; i < s.length(); i++)
  {
    std::cout << s[i] << " : " << std::endl;
  }
  return 0;
};