add_library(CSingleLinkedList CSingleLinkedList.cpp CSingleLinkedList.h)
set_property(TARGET CSingleLinkedList PROPERTY CXX_STANDARD 20)
target_include_directories(CSingleLinkedList PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )