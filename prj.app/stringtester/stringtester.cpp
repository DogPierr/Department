#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "movetest/movetest.h"

const std::vector<int> kTestVector = {10, 100, 1000, 10000, 100000, 1000000};
const std::string fileContent =
    "# Тестирование std::string на предмет правильной move-семантики\n"
    "\n"
    "Было проведено два теста для строки размера **10** и **10000**.\n"
    "Для каждого экземпляра было проведено по 1000 экспериментов move/copy,\n"
    "были замерены времена выполнения этих операций, посчитаны средние и "
    "дисперсии.\n"
    "\n"
    "## Результаты\n"
    "\n"
    "| Размер строки | move среднее | move дисперсия | copy среднее | copy "
    "дисперсия |\n"
    "|---------------|--------------|----------------|--------------|----------"
    "------|\n";

std::string create_string(int size, const std::vector<double>& values) {
  std::stringstream result;
  result << "|";
  result << std::setw(15) << std::setfill(' ') << size << "|";
  for (const auto& value : values) {
    result << std::setw(15) << std::setfill(' ') << value << "|";
  }
  return result.str();
}

void PrintResults(const std::pair<AverageDispersion, AverageDispersion>& res) {
  std::cout << "Среднее move: " << res.first.average << std::endl;
  std::cout << "Дисперсия move: " << res.first.dispersion << std::endl;
  std::cout << "Среднее copy: " << res.second.average << std::endl;
  std::cout << "Дисперсия copy: " << res.second.dispersion << std::endl;
}

void CreateReadme() {
  std::fstream outfile("../prj.app/stringtester/README.md", std::fstream::out);
  outfile << fileContent;
  for (const auto& size : kTestVector) {
    std::string str(size, 'a');
    auto res = TestMoveSemantics(str);
    outfile << create_string(size,
        std::vector<double>{res.first.average, res.first.dispersion,
                            res.second.average, res.second.dispersion}) << std::endl;
  }
  outfile.close();
}

int main() {
  CreateReadme();

  std::string str1(10, 'a');
  std::cout << "Средние значения для строки размера 10: " << std::endl;
  PrintResults(TestMoveSemantics(str1));

  std::string str2(10000, 'a');
  std::cout << "Средние значения для строки размера 10000: " << std::endl;
  PrintResults(TestMoveSemantics(str2));
}