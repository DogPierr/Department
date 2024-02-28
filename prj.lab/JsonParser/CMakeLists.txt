add_library(JsonParser json_parser.cpp json_parser.h)
set_property(TARGET JsonParser PROPERTY CXX_STANDARD 20)
target_include_directories(JsonParser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        )