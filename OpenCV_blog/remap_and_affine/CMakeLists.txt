cmake_minimum_required(VERSION 3.13)
project(remap_and_affine)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories("/usr/local/include/opencv4")
link_libraries("/usr/local/lib")
add_executable(remap_and_affine rotate_image.cpp)
target_link_libraries(remap_and_affine ${OpenCV_LIBS})



