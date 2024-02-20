add_library(CDualLinkedList CDualLinkedList.cpp CDualLinkedList.h)
set_property(TARGET CDualLinkedList PROPERTY CXX_STANDARD 20)
target_include_directories(CDualLinkedList PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )