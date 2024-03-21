#include "movetest/movetest.h"
#include <string>
#include <cassert>

void PrintResults(const std::pair<AverageDispersion, AverageDispersion>& res) {
  std::cout << "Среднее move: " << res.first.average << std::endl;
  std::cout << "Дисперсия move: " << res.first.dispersion << std::endl;
  std::cout << "Среднее copy: " << res.second.average << std::endl;
  std::cout << "Дисперсия copy: " << res.second.dispersion << std::endl;
}

int main() {
  std::string str1(10,'a');
  std::cout << "Средние значения для строки размера 10: " << std::endl;
  PrintResults(TestMoveSemantics(str1));

  std::string str2(10000,'a');
  std::cout << "Средние значения для строки размера 10000: " << std::endl;
  PrintResults(TestMoveSemantics(str2));
}