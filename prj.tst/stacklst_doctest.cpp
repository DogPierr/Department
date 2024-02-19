// 2024 by Polevoi Dmitry under Unlicense

#include <stacklst/stacklst.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("[stacklst] -- copy constructors") {
    Stack<float> st;
    int n = 100;
    for (int i = 0; i < n; ++i) st.Push(i);
    Stack copy(st);
    st = copy;
    for (int i = 0; i < n; ++i) {
        CHECK((n - i - 1) == st.Top());
        st.Pop();
    }
}

TEST_CASE("[stacklst] -- size") {
    int n = 100;
    Stack<float> stack;
    CHECK(stack.Size() == 0);
    for (int i = 0; i < n; ++i) stack.Push(i);
    CHECK(stack.Size() == n);
    for (int i = 0; i < n / 2; ++i) stack.Pop();
    CHECK(stack.Size() == n / 2);
}

TEST_CASE("[stacklst] -- pop top") {
    int n = 100;
    Stack<float> stack;
    for (int i = 0; i < n; ++i) stack.Push(i);
    for (int i = 0; i < n / 2; ++i) {
        CHECK(stack.Top() == (n - i - 1));
        stack.Pop();
    }
}

TEST_CASE("[stacklst] -- move constructors") {
    int n = 100;
    Stack<float> stack;
    for (int i = 0; i < n; ++i) stack.Push(i);
    Stack<float> copy(std::move(stack));
}