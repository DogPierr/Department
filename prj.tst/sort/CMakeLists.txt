find_package(doctest REQUIRED)

add_executable(sort_doctest sort_doctest.cpp)
target_link_libraries(sort_doctest doctest::doctest sort)