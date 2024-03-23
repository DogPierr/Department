find_package(doctest REQUIRED)

add_executable(movetest_doctest movetest_doctest.cpp)
target_link_libraries(movetest_doctest doctest::doctest movetest)