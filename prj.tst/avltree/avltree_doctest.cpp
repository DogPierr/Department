#include "avltree/avltree.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <string>
#define HASH_SIZE 10000
#define SIZE 10000

int intComparator(const int* pElement1, const int* pElement2) {
  if (*pElement1 < *pElement2)
    return 1;
  else if (*pElement1 == *pElement2)
    return 0;
  /*else if (*pElement1 > *pElement2)*/ return -1;
}

struct Some {
  int a;
  int b;
};

int someComparator(const Some* pElement1, const Some* pElement2) {
  if (pElement1->a < pElement2->a)
    return 1;
  else if (pElement1->a == pElement2->a)
    return 0;
  /*else if (*pElement1 > *pElement2)*/ return -1;
}

TEST_CASE("[tree] -- basic add") {
  lab618::CAVLTree<int, &intComparator> tree(HASH_SIZE);
  int* values[SIZE];
  for (int i = 0; i < SIZE; ++i) {
    values[i] = new int;
    *values[i] = i;
    bool res = tree.add(values[i]);
    CHECK(res);
  }
  CHECK(!tree.add(values[0]));
  CHECK(!tree.add(values[1]));
  CHECK(!tree.add(values[2]));
  for (int i = SIZE - 1; i >= 0; --i) {
    auto res = tree.find(*values[i]);
    CHECK(*res == *values[i]);
  }
}

TEST_CASE("[tree] -- remove") {
  lab618::CAVLTree<int, &intComparator> tree(HASH_SIZE);
  int* values[SIZE];
  for (int i = 0; i < SIZE; ++i) {
    values[i] = new int;
    *values[i] = i;
    bool res = tree.add(values[i]);
    CHECK(res);
  }
  CHECK(!tree.remove(SIZE + 1));
  for (int i = 0; i < SIZE; ++i) {
    auto res = tree.remove(*values[i]);
    CHECK(res);
  }
  CHECK(!tree.remove(1));
  CHECK(!tree.remove(2));
  CHECK(!tree.remove(3));
  CHECK(!tree.remove(4));
}

TEST_CASE("[tree] -- update") {
  lab618::CAVLTree<int, &intComparator> tree(HASH_SIZE);
  int* new_int1 = new int;
  *new_int1 = SIZE + 2;
  CHECK(!tree.update(new_int1));
  int* values[SIZE];
  for (int i = 0; i < SIZE; ++i) {
    values[i] = new int;
    *values[i] = i;
    bool res = tree.add(values[i]);
    CHECK(res);
  }
  int* new_int = new int;
  *new_int = SIZE + 5;
  CHECK(!tree.update(new_int));
  for (int i = 0; i < SIZE; ++i) {
    int* new_int = new int;
    *new_int = i;
    CHECK(tree.update(new_int));
  }
}

TEST_CASE("[tree] -- custom structure") {
  lab618::CAVLTree<Some, &someComparator> tree(HASH_SIZE);
  Some* values[SIZE];
  for (int i = 0; i < SIZE; ++i) {
    values[i] = new Some;
    values[i]->a = i;
    bool res = tree.add(values[i]);
    CHECK(res);
  }
  CHECK(!tree.add(values[0]));
  CHECK(!tree.add(values[1]));
  CHECK(!tree.add(values[2]));
  for (int i = SIZE - 1; i >= 0; --i) {
    auto res = tree.find(*values[i]);
    CHECK((res->a == values[i]->a && res->b == values[i]->b));
  }
}
