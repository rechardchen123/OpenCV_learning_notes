cmake_minimum_required(VERSION 3.13)
project(template_matching)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories("/usr/local/include/opencv4")
link_libraries("/usr/local/lib")
add_executable(template_matching template_matching.cpp)
target_link_libraries(template_matching ${OpenCV_LIBS})