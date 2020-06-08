cmake_minimum_required(VERSION 3.13)
project(Hough)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories("/usr/local/include/opencv4")
link_libraries("/usr/local/lib")
add_executable(Hough Hough_circle_transform.cpp)
target_link_libraries(Hough ${OpenCV_LIBS})




