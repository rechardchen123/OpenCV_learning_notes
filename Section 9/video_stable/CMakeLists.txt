cmake_minimum_required(VERSION 3.13)
project(video_stable)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories("/usr/local/include/opencv4")
link_libraries("/usr/local/lib")
add_executable(video_stable video_stable.cpp)
target_link_libraries(video_stable ${OpenCV_LIBS})
