cmake_minimum_required(VERSION 3.13)
project(image_inpaint)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories("/usr/local/include/opencv4")
link_libraries("/usr/local/lib")
add_executable(image_inpaint image_repairing.cpp)
target_link_libraries(image_inpaint ${OpenCV_LIBS})