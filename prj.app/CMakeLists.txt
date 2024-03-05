add_executable(stack_profiler stack_profiler.cpp)
target_link_libraries(stack_profiler stackarr stacklst)

find_package(nlohmann_json 3.7.0 REQUIRED)
add_executable(json_parser jsonreader/jsonreader.cpp)
target_link_libraries(json_parser nlohmann_json::nlohmann_json)