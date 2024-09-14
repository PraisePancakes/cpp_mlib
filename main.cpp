#include "lib/mvector.hpp"
#include "lib/mtest.hpp"
#include "tests/vector/test_vector.hpp"
#include "lib/mstring.hpp"

int main()
{

  mlib::string s = "hello";
  s += "world";

  std::cout << s;

  // test_vector();

  return 0;
};