#include "sort/sort.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <algorithm>
#include <iostream>

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

template <>
int Compare(const CompareInt* pElem1, const CompareInt* pElem2) {
  return pElem2->a - pElem1->a;
}

TEST_CASE("[sort] -- easy") {
  int n = 1000;
  int* a = new int[n];
  for (int i = 0; i < n; i++) {
    a[i] = n - 1 - i;
  }
  int** b_merged = new int*[n];
  int** b_heap = new int*[n];
  int** b_quick = new int*[n];
  for (int i = 0; i < n; i++) {
    b_merged[i] = &a[i];
    b_heap[i] = &a[i];
    b_quick[i] = &a[i];
  }

  templates::mergeSort(b_merged, n, Compare<int>);
  templates::heapSort(b_heap, n, Compare<int>);
  templates::quickSort(b_quick, n, Compare<int>);

  for (int i = 0; i < n; ++i) {
    CHECK(*b_merged[i] == i);
    CHECK(*b_heap[i] == i);
    CHECK(*b_quick[i] == i);
  }
  std::cout << std::endl;
  for (int i = 0; i < n; ++i) {
    CHECK(a[i] == n - 1 - i);
  }
}

TEST_CASE("[sort] -- hard") {
  int n = 100000;
  int* a = new int[n];
  for (int i = 0; i < n; i++) {
    a[i] = i;
  }
  std::random_shuffle(&a[0], &a[n]);
  int** b_merged = new int*[n];
  int** b_heap = new int*[n];
  int** b_quick = new int*[n];
  for (int i = 0; i < n; i++) {
    b_merged[i] = &a[i];
    b_heap[i] = &a[i];
    b_quick[i] = &a[i];
  }
  templates::mergeSort(b_merged, n, Compare<int>);
  templates::heapSort(b_heap, n, Compare<int>);
  templates::quickSort(b_quick, n, Compare<int>);

  for (int i = 0; i < n; ++i) {
    CHECK(*b_merged[i] == i);
    CHECK(*b_heap[i] == i);
    CHECK(*b_quick[i] == i);
  }
}

TEST_CASE("[sort] -- struct") {
  int n = 100000;
  CompareInt* a = new CompareInt[n];
  for (int i = 0; i < n; i++) {
    a[i] = i;
  }
  std::random_shuffle(&a[0], &a[n]);
  CompareInt** b_merged = new CompareInt*[n];
  CompareInt** b_heap = new CompareInt*[n];
  CompareInt** b_quick = new CompareInt*[n];
  for (int i = 0; i < n; i++) {
    b_merged[i] = &a[i];
    b_heap[i] = &a[i];
    b_quick[i] = &a[i];
  }

  templates::mergeSort(b_merged, n, Compare<CompareInt>);
  templates::heapSort(b_heap, n, Compare<CompareInt>);
  templates::quickSort(b_quick, n, Compare<CompareInt>);

  for (int i = 0; i < n; ++i) {
    CHECK(b_merged[i]->a == i);
    CHECK(b_heap[i]->a == i);
    CHECK(b_quick[i]->a == i);
  }
}