cmake_minimum_required (VERSION 2.8)

project(vision)
set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
add_executable(vision main.cpp)
target_link_libraries(vision ${OpenCV_LIBS})
target_link_libraries(vision /home/ubuntu/Documents/ntcore/wpiutil/include)
target_link_libraries(vision /home/ubuntu/Documents/ntcore/include)

MESSAGE(STATUS "OPENCV LIBS" ${OpenCV_LIBS})


