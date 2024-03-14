add_library(mm mm.cpp mm.h)
set_property(TARGET mm PROPERTY CXX_STANDARD 20)
target_include_directories(mm PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )