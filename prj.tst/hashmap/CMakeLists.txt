find_package(doctest REQUIRED)

add_executable(hash_doctest hash_doctest.cpp)
target_link_libraries(hash_doctest doctest::doctest hashmap)