cmake_minimum_required(VERSION 3.10)
project(MPI_Lab VERSION 1.0 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(${MPI_C_INCLUDE_PATH})

add_executable(lab3 lab3.c)
target_link_libraries(lab3 ${MPI_C_LIBRARIES})
target_compile_options(lab3 PRIVATE ${MPI_C_COMPILE_FLAGS})
