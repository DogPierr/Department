#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#define JSON_FILE_NAME "../prj.data/json_parser/data-medium.json"
#define SIZE_OF_FIELD 108
#define IS_DEPLOY false

using json = nlohmann::json;

class sax_event_consumer : public json::json_sax_t {
 public:
//  std::vector<std::string> events;

  bool null() override {
//    "null()";
    return true;
  }

  bool boolean(bool val) override {
//    "boolean(val=" + std::string(val ? "true" : "false") +
//                     ")";
    return true;
  }

  bool number_integer(number_integer_t val) override {
//    "number_integer(val=" + std::to_string(val) + ")";
    return true;
  }

  bool number_unsigned(number_unsigned_t val) override {
//    "number_unsigned(val=" + std::to_string(val) + ")";
    return true;
  }

  bool number_float(number_float_t val, const string_t& s) override {
//    "number_float(val=" + std::to_string(val) + ", s=" + s +
//                     ")";
    return true;
  }

  bool string(string_t& val) override {
//    "string(val=" + val + ")";
    return true;
  }

  bool start_object(std::size_t elements) override {
//    "start_object(elements=" + std::to_string(elements) + ")";
    return true;
  }

  bool end_object() override {
//    "end_object()";
    return true;
  }

  bool start_array(std::size_t elements) override {
//    "start_array(elements=" + std::to_string(elements) + ")";
    return true;
  }

  bool end_array() override {
//    "end_array()";
    return true;
  }

  bool key(string_t& val) override {
//    "key(val=" + val + ")";
    return true;
  }

  bool binary(json::binary_t& val) override {
//    "binary(val=[...])";
    return true;
  }

  bool parse_error(std::size_t position, const std::string& last_token,
                   const json::exception& ex) override {
//    "parse_error(position=" + std::to_string(position) +
//                     ", last_token=" + last_token +
//                     ",\n            ex=" + std::string(ex.what()) + ")";
    return false;
  }
};

void JsonParserFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Can't open file");
  }

  file.seekg(1);
  char* buffer = new char[SIZE_OF_FIELD];
  while (!file.eof()) {
    file.read(buffer, SIZE_OF_FIELD);
  }
  delete[] buffer;
}

void JsonParserVector(const std::string& filename) {
  std::string file_data;
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Can't open file");
  }
  std::getline(file, file_data);

  std::stringstream stream(file_data);
  stream.seekg(1);
  char* buffer = new char[SIZE_OF_FIELD];
  while (!stream.eof()) {
    stream.read(buffer, SIZE_OF_FIELD);
  }

  delete[] buffer;
}

int main() {
  std::fstream file(JSON_FILE_NAME);

  sax_event_consumer sec;

  auto tic = std::chrono::high_resolution_clock::now();
  bool result_sax = json::sax_parse(file, &sec);
  std::cout << "Is SAX parsing successful?" << result_sax << std::endl;
  auto toc = std::chrono::high_resolution_clock::now();

  auto duration_sax =
      std::chrono::duration_cast<std::chrono::microseconds>(toc - tic).count();

  file.close();

  file.open(JSON_FILE_NAME);

  tic = std::chrono::high_resolution_clock::now();
  auto result_dom = json::parse(file);
  toc = std::chrono::high_resolution_clock::now();
  auto duration_dom =
      std::chrono::duration_cast<std::chrono::microseconds>(toc - tic).count();

  tic = std::chrono::high_resolution_clock::now();
  JsonParserFile(JSON_FILE_NAME);
  toc = std::chrono::high_resolution_clock::now();

  auto duration_file_parser =
      std::chrono::duration_cast<std::chrono::microseconds>(toc - tic).count();

  tic = std::chrono::high_resolution_clock::now();
  JsonParserVector(JSON_FILE_NAME);
  toc = std::chrono::high_resolution_clock::now();

  auto duration_vector_parser =
      std::chrono::duration_cast<std::chrono::microseconds>(toc - tic).count();

  std::cout << "Time for SAX: " << duration_sax << " microseconds" << std::endl;
  std::cout << "Time for DOM: " << duration_dom << " microseconds" << std::endl;
  std::cout << "Time for custom file parser: " << duration_file_parser
            << " microseconds" << std::endl;
  std::cout << "Time for custom vector parser: " << duration_vector_parser
            << " microseconds" << std::endl;
}