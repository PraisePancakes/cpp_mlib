#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#include "lib/mvector.hpp"
#include "lib/mtest.hpp"

int main()
{

  static_assert(mlib::char_traits<char>::gt('b', 'a') == true);
  static_assert(mlib::char_traits<char>::lt('a', 'b') == true);
  static_assert(mlib::char_traits<char>::lt('b', 'a') == false);
  static_assert(mlib::char_traits<char>::gt('a', 'b') == false);

  return 0;
}
