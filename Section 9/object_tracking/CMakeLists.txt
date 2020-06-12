cmake_minimum_required(VERSION 3.13)
project(object_tracking)

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

add_example(single_object_tracking)
add_example(multi_objects_tracking)



