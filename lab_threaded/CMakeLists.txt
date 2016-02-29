cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(lab_threaded)

# Find the Threads library
find_package(Threads REQUIRED)

# Set the compiler to use the C++11 standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Create the executable target
add_executable(sample main.cpp)
target_link_libraries(sample "${CMAKE_THREAD_LIBS_INIT}" )