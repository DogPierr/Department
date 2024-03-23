find_package(doctest REQUIRED)

add_executable(CSingleLinkedList_doctest CSingleLinkedList_doctest.cpp)
target_link_libraries(CSingleLinkedList_doctest doctest::doctest CSingleLinkedList)

add_executable(CDualLinkedList_doctest CDualLinkedList_doctest.cpp)
target_link_libraries(CDualLinkedList_doctest doctest::doctest CDualLinkedList)