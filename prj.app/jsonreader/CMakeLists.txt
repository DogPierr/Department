find_package(nlohmann_json 3.7.0 REQUIRED)
add_executable(data jsonreader.cpp)
target_link_libraries(data nlohmann_json::nlohmann_json)