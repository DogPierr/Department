add_library(movetest movetest.cpp movetest.h)
set_property(TARGET movetest PROPERTY CXX_STANDARD 20)
target_include_directories(movetest PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )