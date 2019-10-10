cmake_minimum_required(VERSION 3.0)

project(mycat)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(Boost 1.65 COMPONENTS program_options REQUIRED)

add_executable(mycat main.cpp parse_options.h utils.h)

target_link_libraries(mycat Boost::program_options)