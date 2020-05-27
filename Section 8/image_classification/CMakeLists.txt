cmake_minimum_required(VERSION 3.16)
project(image_classification)

set(CMAKE_CXX_STANDARD 11)


add_executable(image_classification main.cpp)
set(OpenCV_DIR C:/Users/ucesxc0/Downloads/opencv-4.2.0/minGW-build)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(image_classification ${OpenCV_LIBS})