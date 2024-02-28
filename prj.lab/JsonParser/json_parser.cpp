#include "JsonParser/json_parser.h"

JsonParserFile::JsonParserFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open file");
    }

    file.seekg(1);
    char* buffer = new char[SIZE_OF_FIELD];
    while (!file.eof()) {
        json.emplace_back();
        file.read(buffer, SIZE_OF_FIELD);
        std::string str = std::string(buffer, SIZE_OF_FIELD);
        json.back().insert({"user", str.substr(9, 36)});
        json.back().insert({"question", str.substr(59, 36)});
        json.back().insert({"score", str.substr(105, 1)});
    }
    json.pop_back();
    delete[] buffer;
}

const std::unordered_map<std::string, std::string>& JsonParser::operator[](
    size_t index) const {
    return json[index];
}
std::unordered_map<std::string, std::string>& JsonParser::operator[](
    size_t index) {
    return json[index];
}

size_t JsonParser::size() const { return json.size(); }

size_t JsonParser::GetSizeOfField() const { return SIZE_OF_FIELD; }

JsonParserVector::JsonParserVector(const std::string& filename) {
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
        json.emplace_back();
        stream.read(buffer, SIZE_OF_FIELD);
        std::string str = std::string(buffer, SIZE_OF_FIELD);
        json.back().insert({"user", str.substr(9, 36)});
        json.back().insert({"question", str.substr(59, 36)});
        json.back().insert({"score", str.substr(105, 1)});
    }
    json.pop_back();

    delete[] buffer;
}
