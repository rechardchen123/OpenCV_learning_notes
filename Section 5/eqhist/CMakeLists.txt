cmake_minimum_required(VERSION 3.16)
project(eqhist)

set(CMAKE_CXX_STANDARD 11)

add_executable(eqhist main.cpp)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(eqhist ${OpenCV_LIBS})
else(OpenCV_FOUND)
    MESSAGE(FATAL_ERROR "OpenCV library not found")
endif (OpenCV_FOUND)


