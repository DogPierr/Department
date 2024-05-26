#include "movetest/movetest.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

TEST_CASE("[movetest] -- check movetester") {
  MoveTester<true> correct_move;
  auto res_move = TestMoveSemantics(correct_move);
  CHECK(res_move.first.average < res_move.second.average);

  MoveTester<false> incorrect_move;
  auto res_copy = TestMoveSemantics(incorrect_move);
  CHECK(res_copy.first.average > res_copy.second.average);
}