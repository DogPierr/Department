find_package(doctest REQUIRED)

add_executable(stackarrt_doctest stackarrt_doctest.cpp)
target_link_libraries(stackarrt_doctest doctest::doctest stackarrt movetest)

add_executable(stacklstt_doctest stacklstt_doctest.cpp)
target_link_libraries(stacklstt_doctest doctest::doctest stacklstt movetest)