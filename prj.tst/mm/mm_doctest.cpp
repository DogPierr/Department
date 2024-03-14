//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include <doctest/doctest.h>

#include <mm/mm.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

TEST_CASE("[mm] --") {
  lab618::CMemoryManager<int> memoryManager(10);
}