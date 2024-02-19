#include <stackarr/stackarr.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

TEST_CASE("[stackarr] -- copy constructors") {
    StackArray<float> st;
    int n = 100;
    for (int i = 0; i < n; ++i) st.Push(i);
    StackArr copy(st);
    st = copy;
    for (int i = 0; i < n; ++i) {
        CHECK((n - i - 1) == st.Top());
        st.Pop();
    }
}

TEST_CASE("[stackarr] -- size") {
    int n = 100;
    StackArray<float> stack;
    CHECK(stack.Size() == 0);
    for (int i = 0; i < n; ++i) stack.Push(i);
    CHECK(stack.Size() == n);
    for (int i = 0; i < n / 2; ++i) stack.Pop();
    CHECK(stack.Size() == n / 2);
}

TEST_CASE("[stackarr] -- pop top") {
    int n = 100;
    StackArray<float> stack;
    for (int i = 0; i < n; ++i) stack.Push(i);
    for (int i = 0; i < n / 2; ++i) {
        CHECK(stack.Top() == (n - i - 1));
        stack.Pop();
    }
}

TEST_CASE("[stackarr] -- move constructors") {
    int n = 100;
    StackArray<float> stack;
    for (int i = 0; i < n; ++i) stack.Push(i);
    StackArray<float> copy(std::move(stack));
}