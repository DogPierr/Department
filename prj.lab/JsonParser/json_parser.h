#ifndef MIPT2024S_3_JSON_PARSER_H
#define MIPT2024S_3_JSON_PARSER_H

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

class JsonParser {
 public:
  virtual ~JsonParser() = default;

  const std::unordered_map<std::string, std::string>& operator[](
      size_t index) const;
  std::unordered_map<std::string, std::string>& operator[](size_t index);

  size_t size() const;
  size_t GetSizeOfField() const;

 protected:
  std::vector<std::unordered_map<std::string, std::string>> json;
  const size_t SIZE_OF_FIELD = 108;
};

class JsonParserFile : public JsonParser {
 public:
  JsonParserFile() = default;
  JsonParserFile(const std::string& filename);
};

class JsonParserVector : public JsonParser {
 public:
  JsonParserVector() = default;
  JsonParserVector(const std::string& filename);
};

#endif  // MIPT2024S_3_JSON_PARSER_H
