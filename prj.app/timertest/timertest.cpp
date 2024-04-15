#include "timer/timer.h"
#include <iostream>

bool flag = false;

#include "stackarr/stackarr.hpp"

#define test false

StackArr stack;

void push() {
#ifdef test
  static Ancor ancor(__COUNTER__, __FUNCTION__, flag);
  Timer timer(ancor.GetPtr(), flag);
#endif

  stack.Push(1.0);
}

void pop() {
#ifdef test
  static Ancor ancor(__COUNTER__, __FUNCTION__, flag);
  Timer timer(ancor.GetPtr(), flag);
#endif

  stack.Pop();
}

FileTimer a[__COUNTER__];
FileTimer* Ancor::data = a;

int main() {
#if test
  printf("With profiler!\n");
#else
  printf("Without profiler!\n");
#endif

  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 100000; ++i) push();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
  if (!flag) printf("push,%li,100000\n", duration);

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 100000; ++i) pop();
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
  if (!flag) printf("pop,%li,100000\n", duration);

  Ancor t(__COUNTER__ - 1, flag);
  t.WriteToFile("../prj.app/timertest/log.csv");
}

