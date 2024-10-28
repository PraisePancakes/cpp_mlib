#include <iostream>
#include "lib/mstack.hpp"
#include <stack>
int main()
{
  mlib::stack<int> s;
  s.push(4);
  s.push(6);
  s.push(1);

  while (!s.empty())
  {
    std::cout << s.top();
    s.pop();
  }
  return 0;
};
