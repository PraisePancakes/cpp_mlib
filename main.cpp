#include <iostream>
#include <memory>
#include "lib/mallocator.hpp"

int main()
{
  mlib::allocator<int> a;

  auto p = mlib::allocator_traits<mlib::allocator<int>>::allocate(a, 1);
  p = mlib::allocator_traits<mlib::allocator<int>>::reallocate(a, p, 5);

  return 0;
};
