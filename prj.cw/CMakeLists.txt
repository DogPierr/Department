find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( junction junctionpoints/junction.cpp junctionpoints/junction.h)
target_link_libraries( junction ${OpenCV_LIBS} )