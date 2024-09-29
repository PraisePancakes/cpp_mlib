#include <iostream>
#include "lib/mvector.hpp"
#include "lib/mstring.hpp"
#include "lib/mttraits.hpp"
#include <ctime>
#include <chrono>

template <typename T>
T sum(T a)
{
  return a;
};

template <typename T, typename = mlib::enable_if<std::is_integral<T>::value>::type, typename... Args>
T sum(T a1, Args... a)
{
  return a1 + sum(a...);
};

int main()
{
  std::cout << sum("a", 2, 3);
  std::cout << sum(1, 2, 3);
  return 0;
}