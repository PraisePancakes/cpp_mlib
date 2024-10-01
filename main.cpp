#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include "lib/mttraits.hpp"
#include <ctime>
#include <chrono>
#include <algorithm>

static_assert(!mlib::is_member_ptr<int *>::value);

struct S
{
  int i{42};
  int foo() { return 0xF00; }
};
using mem_int_ptr_t = int S::*;
using mem_fun_ptr_t = int (S::*)();

static_assert(mlib::is_member_ptr<mem_int_ptr_t>::value);
static_assert(mlib::is_member_ptr<mem_fun_ptr_t>::value);

int main()
{
  int a = 1;
  int b = 2;
  int *p1 = &a;
  int *p2 = &b;

  mlib::vec<int *> v{p1, p2};
  std::vector<int *> v1{p1, p2};

  for (std::vector<int *>::iterator i = v1.begin(); i != v1.end(); ++i)
  {
    std::cout << **i << std::endl;
  };

  for (mlib::vec<int *>::iterator i = v.mbegin(); i != v.mend(); ++i)
  {
    std::cout << **i << std::endl;
  };

  return 0;
}