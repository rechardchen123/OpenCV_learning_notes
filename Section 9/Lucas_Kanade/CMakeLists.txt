cmake_minimum_required(VERSION 3.13)
project(Lucas_Kanade)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories("/usr/local/include/opencv4")
link_libraries("/usr/local/lib")
add_executable(Lucas_Kanade LK.cpp)
target_link_libraries(Lucas_Kanade ${OpenCV_LIBS})