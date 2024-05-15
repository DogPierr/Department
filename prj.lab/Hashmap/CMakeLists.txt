add_library(hashmap hash.cpp hash.h)
set_property(TARGET hashmap PROPERTY CXX_STANDARD 20)
target_include_directories(hashmap PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )

add_library(hashmap_no_mm hash_new.cpp hash_new.h)
set_property(TARGET hashmap_no_mm PROPERTY CXX_STANDARD 20)
target_include_directories(hashmap_no_mm PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include> )