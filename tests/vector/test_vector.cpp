#include "test_vector.hpp"
#include "../../lib/vector.hpp"
#include "../../lib/test.hpp"

using _vec = mlib::vec<int>;

void test_push()
{
  MTest<_vec, _vec> t({0, 1, 2}, {0, 1, 2, 3});
  auto tlambda = [&]() -> _vec
  {
    t._MTEST_input.push_back(3);
    return t._MTEST_input;
  };
  t.test(tlambda);
};

void test_pop()
{
  MTest<_vec, _vec> t({0, 1, 2, 3}, {0, 1, 2});
  auto tlambda = [&]() -> _vec
  {
    t._MTEST_input.pop_back();
    return t._MTEST_input;
  };

  t.test(tlambda);
};

void test_empty()
{
  MTest<_vec, bool> t({}, true);
  auto tlambda = [&]() -> bool
  {
    return t._MTEST_input.empty();
  };

  t.test(tlambda);
}

void test_vector()
{
  test_empty();
  test_push();
  test_pop();
};
