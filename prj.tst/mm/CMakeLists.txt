find_package(doctest REQUIRED)

add_executable(mm_doctest mm_doctest.cpp)
target_link_libraries(mm_doctest doctest::doctest mm)