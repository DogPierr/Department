 #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <mm/mm.h>

struct Some {
  int* ptr;
  int* new_ptr;
};


TEST_CASE("[mm] -- basic") {
  lab618::CMemoryManager<float> memoryManager(5, true);
  float** p_s = new float*[100];
  for (int i = 0; i < 100; ++i) {
    p_s[i] = memoryManager.newObject();
    *p_s[i] = 5.0f;
  }
  for (int i = 0; i < 100; i+=2 ){
    memoryManager.deleteObject(p_s[i]);
  }
  auto p = memoryManager.newObject();
  *p = 10.0f;
  auto p_1 = memoryManager.newObject();
  *p_1 = 10.0f;
  auto p_2 = memoryManager.newObject();
  *p_2 = 10.0f;
  memoryManager.clear();

  int a = 100;
}

TEST_CASE("[mm] -- struct") {
  lab618::CMemoryManager<Some> memoryManager(5, true);
  Some** p_s = new Some*[100];
  for (int i = 0; i < 100; ++i) {
    p_s[i] = memoryManager.newObject();
//    *p_s[i]->ptr = 5;
  }
  for (int i = 0; i < 100; i+=2 ){
    memoryManager.deleteObject(p_s[i]);
  }
  auto p = memoryManager.newObject();
//  *p->ptr = 10.0f;
  auto p_1 = memoryManager.newObject();
//  *p_1->ptr = 10.0f;
  auto p_2 = memoryManager.newObject();
//  *p_2->ptr = 10.0f;
  memoryManager.clear();

  int a = 100;
}