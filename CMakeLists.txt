cmake_minimum_required(VERSION 3.13)
project(bitonic_sort)

#set(CMAKE_CXX_STANDARD 14)

#Bring the headers (.h) into the project
include_directories(include)
#Bring the source files (.cpp) into the project
include_directories(src)

add_library(utils STATIC src/utils.cpp)
add_library(parser STATIC src/input_parser.cpp)
add_library(merge STATIC src/bitonic_merge.cpp)

add_executable(bitonic_sort src/main.cpp)

# Require MPI for this project:
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(bitonic_sort merge utils parser ${MPI_LIBRARIES})
