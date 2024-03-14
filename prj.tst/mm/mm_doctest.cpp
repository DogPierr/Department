 #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
 #include <doctest/doctest.h>

#include <mm/mm.h>


TEST_CASE("[mm] -- basic") {
  lab618::CMemoryManager<float> memoryManager(5, false);
  float** p_s = new float*[100];
  for (int i = 0; i < 100; ++i) {
    p_s[i] = memoryManager.newObject();
    *p_s[i] = 10.0f;
  }
  for (int i = 0; i < 100; i+=2 ){
    memoryManager.deleteObject(p_s[i]);
  }
  memoryManager.clear();

  int a = 100;
}