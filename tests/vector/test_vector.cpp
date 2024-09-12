#include "test_vector.hpp"
#include "../../lib/vector.hpp"
#include "../../lib/test.hpp"

using _vec = mlib::vec<int>;

void test_push()
{
  MTest<_vec, _vec> t({0, 1, 2});
  auto tlambda = [&t]() -> _vec
  {
    t.input().push_back(3);
    return t.input();
  };
  t.test(tlambda, {0, 1, 2, 3});

};

void test_pop()
{
  MTest<_vec, _vec> t({0, 1, 2, 3});
  auto tlambda = [&t]() -> _vec
  {
    t.input().pop_back();
    return t.input();
  };

  t.test(tlambda, {0, 1, 2});
};

void test_empty()
{
  MTest<_vec, bool> t({});
  auto tlambda = [&]() -> bool
  {
    return t.input().empty();
  };

  t.test(tlambda, true);
}

void test_vector()
{
  test_empty();
  test_push();
  test_pop();
};
