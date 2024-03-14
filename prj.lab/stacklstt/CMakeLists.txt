add_library(stacklstt stacklstt.cpp stacklstt.h)
set_property(TARGET stacklstt PROPERTY CXX_STANDARD 20)
target_include_directories(stacklstt PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )