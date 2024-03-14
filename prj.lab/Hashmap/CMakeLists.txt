add_library(hashmap hash_new.cpp hash_new.h)
set_property(TARGET hashmap PROPERTY CXX_STANDARD 20)
target_include_directories(hashmap PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )