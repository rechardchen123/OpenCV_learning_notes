cmake_minimum_required(VERSION 3.13)
project(image_classification)

set(CMAKE_CXX_STANDARD 14)

set(OpenCV_DIR /usr/local/share/opencv4)
if(MSVC)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
MACRO(add_example name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS})
ENDMACRO()

add_example(image_classification_caffe)
add_example(image_classification_tensorflow)
