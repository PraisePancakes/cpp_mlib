#include <iostream>
#include <cstring>
#include <vector>
#include "lib/mdeque.hpp"
#include "lib/mvector.hpp"
template <typename T>
class my_deque
{
  std::vector<T *> map;
  size_t start;
  size_t finish;
  static inline size_t CHUNK_SIZE = 8;

  size_t sz = 0;

public:
  my_deque() : start(0), finish(0), sz(0)
  {
    map.reserve(8);
    map[0] = new T[CHUNK_SIZE];
    start = CHUNK_SIZE / 2;
    finish = start;
  };

  void push_back(const T &val)
  {
    if (finish >= CHUNK_SIZE)
    {
      map.push_back(new T[CHUNK_SIZE]);
      finish = 0;
    };
    sz++;
    map[map.size() - 1][finish++] = val;
  };

  void push_front(const T &val)
  {

    if (start == 0)
    {

      map.insert(map.begin(), new T[CHUNK_SIZE]);
      start = CHUNK_SIZE;
    };

    sz++;
    map[0][--start] = val;
  };

  size_t size() const
  {
    return this->sz;
  };

  T &operator[](const size_t index)
  {
    const size_t block = (start + index) / CHUNK_SIZE;
    const size_t offset = (start + index) % CHUNK_SIZE;

    return map[block][offset];
  };

  ~my_deque() {};
};

int main()
{
  mlib::deque<int> d;

  d.push_back(10);
  d.push_front(6);
  d.push_back(8);

  for (size_t i = 0; i < d.size(); i++)
  {
    std::cout << d[i];
  }
  return 0;
}
