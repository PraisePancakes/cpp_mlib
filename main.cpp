#include <iostream>
#include <cstring>
#include "lib/mstring.hpp"
#define buff_size 10

size_t size(const char *s)
{
  size_t size = 0;
  while (*s != '\0')
  {
    s++;
    size++;
  };

  return size;
}

class S
{
  union U
  {
    char b[buff_size];
    char *h;
  };

  bool using_stack; // This flag will track whether we are using `u.b` or `u.h`

public:
  U u;
  S(const char *s)
  {
    size_t size_s = size(s);

    if (size_s < buff_size)
    {
      using_stack = true;
      std::cout << size_s;
      for (size_t i = 0; i < size_s; i++)
      {
        u.b[i] = s[i];
      }
      u.b[size_s] = '\0';
    }
    else
    {
      using_stack = false;
      u.h = (char *)malloc(size_s + 1);
      for (size_t i = 0; i < size_s; i++)
      {
        u.h[i] = s[i];
      }
      u.h[size_s] = '\0';
    };
  }

  ~S() // Destructor to free memory if heap allocation was done
  {
    if (!using_stack && u.h != nullptr)
    {
      free(u.h);
    }
  }

  friend std::ostream &operator<<(std::ostream &s, const S &obj)
  {
    if (obj.using_stack)
    {
      s << obj.u.b;
    }
    else
    {
      s << obj.u.h;
    }

    return s;
  };
};

int main()
{
  mlib::string s("hel");

  std::cout << s.size() << std::endl;

  std::cout << s << std::endl;

  return 0;
}
