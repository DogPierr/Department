add_library(stackarrt stackarrt.cpp stackarrt.hpp)
set_property(TARGET stackarrt PROPERTY CXX_STANDARD 20)
target_include_directories(stackarrt PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )