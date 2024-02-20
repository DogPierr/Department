#include "CSingleLinkedList/CSingleLinkedList.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

TEST_CASE("[CSingleLinkedList] -- copy constructors") {
  lab618::CSingleLinkedList<int> lst;
  int n = 100;
  for (int i = 0; i < n; ++i) lst.pushFront(i);
  for (int i = 0; i < n; ++i) {
    CHECK((n - i - 1) == lst.popFront());
  }
}

TEST_CASE("[CSingleLinkedList] -- iterators") {
  int n = 100;
  lab618::CSingleLinkedList<int> lst;
  for (int i = 0; i < n; ++i) lst.pushBack(i);
  lab618::CSingleLinkedList<int>::CIterator it = lst.begin();
  for (int i = 0; i < n; ++i) {
    CHECK(i == *it);
    lst.erase(it);
    ++it;
  }
}

TEST_CASE("[CSingleLinkedList] -- iterators backwards") {
  int n = 100;
  lab618::CSingleLinkedList<int> lst;
  for (int i = 0; i < n; ++i) lst.pushBack(i);
  lab618::CSingleLinkedList<int>::CIterator it = lst.begin();
  for (int i = 0; i < n - 1; ++i) {
    ++it;
  }
  for (int i = 0; i < n; ++i) {
    CHECK((n - i - 1) == *it);
    lst.erase(it);
  }
}

TEST_CASE("[CSingleLinkedList] -- move constructors") {
  int n = 100;
  lab618::CSingleLinkedList<int> lst;
  for (int i = 0; i < n; ++i) lst.pushBack(i);
  CHECK(lst.getSize() == n);
  lab618::CSingleLinkedList<int>::CIterator it = lst.begin();
  ++it;
  lst.erase(it);
  CHECK(lst.popFront() == 0);
  CHECK(lst.popFront() == 2);
  CHECK(lst.popFront() == 3);
  CHECK(lst.popFront() == 4);
  CHECK(lst.getSize() == n - 5);
  lst.clear();
  CHECK(lst.getSize() == 0);
}