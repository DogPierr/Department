#include "timer/timer.h"
#include <iostream>


void A(std::string s) {
  Timer timer(__FUNCTION__ );
  for (int i = 0; i < 100; ++i) {
    std::cout << "iteration: " << i << std::flush;
  }
  std::cout << std::endl;
}

void B(std::string& s) {
  Timer timer(__FUNCTION__ );
  for (int i = 0; i < 1000; ++i) {
    std::cout << "iteration: " << i << std::flush;
  }
  std::cout << std::endl;
}

int main() {
  std::string s(1, 'a');
  A(s);
  B(s);
  B(s);
  Timer t("");
  t.WriteToFile("../prj.app/timertest/log.csv");
}

