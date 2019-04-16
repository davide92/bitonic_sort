cmake_minimum_required(VERSION 3.13)
project(bitonic_sort)

set(CMAKE_CXX_STANDARD 14)

#Bring the headers (.h) into the project
include_directories(include)
#Bring the source files (.cpp) into the project
include_directories(src)
#Bring the mpi header into the project
include_directories(/home/davide/.openmpi/include)

find_library(C_OPENMPI_LIB NAMES libmpi.so PATHS /home/davide/.openmpi/lib)

add_library(utils STATIC src/utils.cpp)
add_library(parser STATIC src/input_parser.cpp)

add_executable(bitonic_sort src/main.cpp)

target_link_libraries(bitonic_sort utils parser)
# for future reference, link gurobi separately from other libraries, no idea why
target_link_libraries(bitonic_sort ${C_OPENMPI_LIB})