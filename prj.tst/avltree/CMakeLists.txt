find_package(doctest REQUIRED)

add_executable(avltree_doctest avltree_doctest.cpp)
target_link_libraries(avltree_doctest doctest::doctest avltree)