add_library(sort sort.cpp sort.h)
set_property(TARGET sort PROPERTY CXX_STANDARD 20)
target_include_directories(sort PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )