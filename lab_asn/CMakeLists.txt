cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(lab_asn)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(writer writer.cpp report.cpp)
target_link_libraries(writer "${CMAKE_THREAD_LIBS_INIT}" rt)

add_executable(reader reader.cpp report.cpp)
target_link_libraries(reader "${CMAKE_THREAD_LIBS_INIT}" rt)

install(TARGETS writer RUNTIME DESTINATION bin)

