cmake_minimum_required(VERSION 3.13)
project(kalman_filter)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories("/usr/local/include/opencv4")
link_libraries("/usr/local/lib")
add_executable(kalman_filter kalman_filter.cpp)
target_link_libraries(kalman_filter ${OpenCV_LIBS})
