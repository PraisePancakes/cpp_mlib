#include "test_vector.hpp"
#include "../../lib/mvector.hpp"
#include "../../lib/mtest.hpp"

using _vec = mlib::vec<int>;

void test_push()
{
  MTest<_vec, _vec> t({0, 1, 2}, "Vector");
  auto tlambda = [&t]() -> _vec
  {
    t.input().push_back(3);
    return t.input();
  };
  t.test(tlambda, {0, 1, 2, 3}, "push_back 3");
};

void test_pop()
{
  MTest<_vec, _vec> t({0, 1, 2, 3}, "Vector");
  auto tlambda = [&t]() -> _vec
  {
    t.input().pop_back();
    return t.input();
  };

  t.test(tlambda, {0, 1, 2}, "pop_back x1");
};

void test_empty()
{
  MTest<_vec, bool> t({}, "Vector");
  auto tlambda = [&]() -> bool
  {
    return t.input().empty();
  };

  t.test(tlambda, true, "test if empty");
}

void test_slice()
{
  MTest<_vec, _vec> t({0, 1, 2}, "Vector");

  auto tlambda = [&]() -> _vec
  {
    return t.input().slice(0, 1);
  };

  t.test(tlambda, {0}, "slice from one to other");
};

void test_vector()
{
  test_slice();
  test_empty();
  test_push();
  test_pop();
};
