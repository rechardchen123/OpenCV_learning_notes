cmake_minimum_required(VERSION 3.16)
project(assignment1)

set(CMAKE_CXX_STANDARD 11)

add_executable(assignment1  main.cpp)
FIND_PACKAGE(OpenCV REQUIRED)
IF (OpenCV_FOUND)
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(assignment1 ${OpenCV_LIBS})
ELSE (OpenCV_FOUND)
    MESSAGE(FATAL_ERROR "OpenCV library not found")
ENDIF (OpenCV_FOUND)