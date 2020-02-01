cmake_minimum_required(VERSION 3.8)


project(OpenCVWIN32)

enable_language(C)
enable_language(CXX)

include_directories("include" "${OpenCV_DIR}/include")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Install prefix" FORCE)

file(GLOB sources "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB sourcespp "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_library(opencv_world343.lib)
add_library(opencv_world343d.lib)
add_library(yolo_cpp_dll_no_gpu.lib)

include_directories(${OpenCV_INCLUDE_DIRS})

find_package(OpenCV)

add_executable(OpenCVWIN32 ${sources} ${sourcespp} ${headers})