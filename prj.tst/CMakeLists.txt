find_package(doctest REQUIRED)

add_executable(stackarr_doctest stack/stackarr_doctest.cpp)
target_link_libraries(stackarr_doctest doctest::doctest stackarr)

add_executable(stackarrt_doctest stackt/stackarrt_doctest.cpp)
target_link_libraries(stackarrt_doctest doctest::doctest stackarrt movetest)

add_executable(stacklst_doctest stack/stacklst_doctest.cpp)
target_link_libraries(stacklst_doctest doctest::doctest stacklst)

add_executable(stacklstt_doctest stackt/stacklstt_doctest.cpp)
target_link_libraries(stacklstt_doctest doctest::doctest stacklstt movetest)

add_executable(CSingleLinkedList_doctest linkedlist/CSingleLinkedList_doctest.cpp)
target_link_libraries(CSingleLinkedList_doctest doctest::doctest CSingleLinkedList)

add_executable(CDualLinkedList_doctest linkedlist/CDualLinkedList_doctest.cpp)
target_link_libraries(CDualLinkedList_doctest doctest::doctest CDualLinkedList)

add_executable(hash_doctest hashmap/hash_doctest.cpp)
target_link_libraries(hash_doctest doctest::doctest hashmap)

add_executable(mm_doctest mm/mm_doctest.cpp)
target_link_libraries(mm_doctest doctest::doctest mm)

add_executable(movetest_doctest movetester/movetest_doctest.cpp)
target_link_libraries(movetest_doctest doctest::doctest movetest)
