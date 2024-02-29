#include "JsonParser/json_parser.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#define JSON_FILE_NAME "../prj.data/json_parser/data-small.json"
#define JSON_TEST_FILE "../prj.data/json_parser/file.json"

#define TEST_CREATE_NLOCHMAN "../prj.data/json_parser/nlochman.csv"
#define TEST_CREATE_MY_PARSER "../prj.data/json_parser/my_parser.csv"
#define TEST_CREATE_MY_PARSER_VECTOR \
  "../prj.data/json_parser/my_parser_vector.csv"

#define TEST_MANIPULATE_NLOCHMAN \
  "../prj.data/json_parser/nlochman_manipulate.csv"
#define TEST_MANIPULATE_MY_PARSER \
  "../prj.data/json_parser/my_parser_manipulate.csv"
#define TEST_MANIPULATE_MY_PARSER_VECTOR \
  "../prj.data/json_parser/my_parser_vector_manipulate.csv"

#define MAX_SIZE 10'000

const std::string str =
    "{\"user\":\"e213cc2b-387e-4d7d-983c-8abc19a586b1\",\"question\":"
    "\"d3bdb068-7245-4521-ae57-d0e9692cb627\",\"score\":1}";

using json = nlohmann::json;

void AddToFile(std::fstream& file, const std::string& str) {
  file.seekg(0, std::ios::end);
  int length = file.tellg();
  if (length > 0) {
    file.seekg(length - 1, std::ios::beg);
    file << ',' << str << ']';
  } else {
    file.seekg(0, std::ios::beg);
    file << '[' << str << ']';
  }
  file.seekg(0, std::ios::beg);
}

void TestBuildSpeed() {
  std::ofstream ofs;
  ofs.open(JSON_TEST_FILE, std::ofstream::out | std::ofstream::trunc);
  ofs.close();

  std::fstream nlochman_file(TEST_CREATE_NLOCHMAN,
                             std::fstream::out | std::fstream::trunc);
  nlochman_file << "n,time" << std::endl;
  std::fstream my_parser_file(TEST_CREATE_MY_PARSER,
                              std::fstream::out | std::fstream::trunc);
  my_parser_file << "n,time" << std::endl;
  std::fstream my_parser_vector_file(TEST_CREATE_MY_PARSER_VECTOR,
                                     std::fstream::out | std::fstream::trunc);
  my_parser_vector_file << "n,time" << std::endl;

  std::fstream file(JSON_TEST_FILE);
  std::cout << "Starting building test..." << std::endl;
  for (int i = 0; i < MAX_SIZE; ++i) {
    if (i % 1000 == 0) {
      std::cout << "\rTesting... " << i + 1 << '/' << MAX_SIZE << std::flush;
    }
    AddToFile(file, str);

    auto tic = std::chrono::high_resolution_clock::now();
    json data = json::parse(file);
    auto toc = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(toc - tic)
            .count();
    nlochman_file << i + 1 << ',' << duration << std::endl;

    tic = std::chrono::high_resolution_clock::now();
    JsonParserFile parser(JSON_TEST_FILE);
    toc = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(toc - tic)
                   .count();
    my_parser_file << i + 1 << ',' << duration << std::endl;

    tic = std::chrono::high_resolution_clock::now();
    JsonParserVector parser_vector(JSON_TEST_FILE);
    toc = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(toc - tic)
                   .count();
    my_parser_vector_file << i + 1 << ',' << duration << std::endl;
  }
}

void TestManipulationSpeed() {
  std::fstream nlochman_file(TEST_MANIPULATE_NLOCHMAN,
                             std::fstream::out | std::fstream::trunc);
  nlochman_file << "n,time" << std::endl;
  std::fstream my_parser_file(TEST_MANIPULATE_MY_PARSER,
                              std::fstream::out | std::fstream::trunc);
  my_parser_file << "n,time" << std::endl;
  std::fstream my_parser_vector_file(TEST_MANIPULATE_MY_PARSER_VECTOR,
                                     std::fstream::out | std::fstream::trunc);
  my_parser_vector_file << "n,time" << std::endl;

  std::fstream file(JSON_FILE_NAME);
  json data = json::parse(file);

  JsonParserFile parser(JSON_FILE_NAME);

  JsonParserVector parser_vector(JSON_FILE_NAME);

  auto size = data.size();
  std::vector<std::string> fields({"user", "question", "score"});

  std::cout << "Starting manipulation test..." << std::endl;
  for (int i = 0; i < MAX_SIZE; ++i) {
    int index = rand() % size;
    int field = rand() % fields.size();

    auto tic = std::chrono::high_resolution_clock::now();
    auto res_1 = data[index][fields[field]];
    auto toc = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic).count();
    nlochman_file << i + 1 << ',' << duration << std::endl;

    tic = std::chrono::high_resolution_clock::now();
    auto res_2 = parser[index][fields[field]];
    toc = std::chrono::high_resolution_clock::now();

    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic).count();
    my_parser_file << i + 1 << ',' << duration << std::endl;

    tic = std::chrono::high_resolution_clock::now();
    auto res_3 = parser_vector[index][fields[field]];
    toc = std::chrono::high_resolution_clock::now();

    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic).count();
    my_parser_vector_file << i + 1 << ',' << duration << std::endl;

    if (i % 1000 == 0) {
      std::cout << "\rTesting... " << i + 1 << '/' << MAX_SIZE << " " << res_1
                << " " << res_2 << " " << res_3 << std::flush;
    }
  }
}

void TestBuildFile() {
  std::fstream file(JSON_FILE_NAME);

}

int main() {
  TestBuildSpeed();
  TestManipulationSpeed();
}