#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"
#include <iterator>
#include "lib/mstring.hpp"

struct Foo
{
  Foo() {};
  ~Foo() { std::cout << "destructing Foo" << std::endl; };
};

void destroy_foo(Foo *f)
{
  f->~Foo();
};

Foo &get_foo(std::vector<Foo> &fv, size_t index)
{
  return fv[index];
};

int main()
{

  Foo f1;
  std::vector<Foo> v;
  v.push_back(f1);

  destroy_foo(&(get_foo(v, 0)));

  return 0;
}