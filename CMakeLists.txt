cmake_minimum_required (VERSION 2.8)

project(vision)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
add_executable(vision main.cpp)
target_link_libraries(vision ${OpenCV_LIBS})
MESSAGE(STATUS "OPENCV LIBS" ${OpenCV_LIBS})

