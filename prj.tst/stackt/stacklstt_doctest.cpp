#include "stacklstt/stacklstt.h"
#include "movetest/movetest.h"
#include "string"
#include "vector"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


TYPE_TO_STRING(std::vector<int>);
TYPE_TO_STRING(std::string);

TEST_CASE_TEMPLATE("[stacklstt] -- copy constructors",
                   T, std::string, std::vector<int>) {
  StackLstT<T> st;
  int n = 100;
  for (int i = 0; i < n; ++i) st.Push(T());
  Stack copy(st);
  st = copy;
  for (int i = 0; i < n; ++i) {
    st.Pop();
  }
}

TEST_CASE_TEMPLATE("[stacklstt] -- size",
                   T, std::string, std::vector<int>) {
  int n = 100;
  StackLstT<T> stack;
  CHECK(stack.Size() == 0);
  for (int i = 0; i < n; ++i) stack.Push(T());
  CHECK(stack.Size() == n);
  for (int i = 0; i < n / 2; ++i) stack.Pop();
  CHECK(stack.Size() == n / 2);
}

TEST_CASE("[stacklstt] -- move constructor") {
  StackLstT<std::string> stack;

  for (int i = 0; i < 1000; ++i) stack.Push(std::string(100, 'a'));

  CHECK(TestMoveSemantics<StackLstT<std::string>>(stack));
}