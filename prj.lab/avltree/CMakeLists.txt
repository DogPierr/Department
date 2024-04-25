add_library(avltree avltree.cpp avltree.h)
set_property(TARGET avltree PROPERTY CXX_STANDARD 20)
target_include_directories(avltree PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )