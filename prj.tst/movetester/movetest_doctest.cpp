#include "movetest/movetest.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

TEST_CASE("[movetest] -- check movetester") {
  MoveTester<true> correct_move;
  CHECK(TestMoveSemantics(correct_move));

  MoveTester<false> incorrect_move;
  CHECK(!TestMoveSemantics(incorrect_move));
}