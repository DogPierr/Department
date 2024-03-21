#include "sort/sort.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <iostream>
#include <algorithm>

template <typename T>
int Compare(const T* pElem1, const T* pElem2) {
  return *pElem2 - *pElem1;
}

struct CompareInt {
  int a;
  CompareInt() : a(0) {}
  CompareInt& operator=(int b) {
    a = b;
    return *this;
  }
};

template<>
int Compare(const CompareInt* pElem1, const CompareInt* pElem2) {
  return pElem2->a - pElem1->a;
}

TEST_CASE("[sort] -- easy") {
  int n = 1000;
  int* a = new int[n];
  for (int i = 0; i < n; i++) {
    a[i] = n - 1 - i;
  }
  int** b = new int*[n];
  for (int i = 0; i < n; i++) {
    b[i] = &a[i];
  }

  templates::mergeSort(b, n, Compare<int>);

  for (int i = 0; i < n; ++i) {
    std::cout << *b[i] << " ";
    CHECK(*b[i] == i);
  }
  std::cout << std::endl;
  for (int i = 0; i < n; ++i) {
    CHECK(a[i] == n - 1 - i);
  }
}

TEST_CASE("[sort] -- hard") {
  int n = 10000;
  int* a = new int[n];
  for (int i = 0; i < n; i++) {
    a[i] = i;
  }
  std::random_shuffle(&a[0], &a[n]);
  int** b = new int*[n];
  for (int i = 0; i < n; i++) {
    b[i] = &a[i];
  }
  templates::mergeSort(b, n, Compare<int>);
  for (int i = 0; i < n; ++i) {
    CHECK(*b[i] == i);
  }
}

TEST_CASE("[sort] -- struct") {
  int n = 10000;
  CompareInt* a = new CompareInt[n];
  for (int i = 0; i < n; i++) {
    a[i] = i;
  }
  std::random_shuffle(&a[0], &a[n]);
  CompareInt** b = new CompareInt*[n];
  for (int i = 0; i < n; i++) {
    b[i] = &a[i];
  }
  templates::mergeSort(b, n, Compare<CompareInt>);
  for (int i = 0; i < n; ++i) {
    CHECK(b[i]->a == i);
  }
}