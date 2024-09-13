#include "lib/mvector.hpp"
#include "lib/mtest.hpp"
#include "tests/vector/test_vector.hpp"
#include "lib/mstring.hpp"

int main()
{

  // test_vector();

  mlib::string s("hello");
  s[0] = 'a';
  std::cout << s;
  return 0;
};