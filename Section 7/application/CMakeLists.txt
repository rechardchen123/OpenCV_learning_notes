cmake_minimum_required(VERSION 3.16)
project(application)

set(CMAKE_CXX_STANDARD 11)


#add_executable(application doc_alignment.cpp)
#add_executable(application Panoramic_image.cpp)
add_executable(application find_object.cpp)
set(OpenCV_DIR C:/Users/ucesxc0/Downloads/opencv-4.2.0/minGW-build)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(application ${OpenCV_LIBS})