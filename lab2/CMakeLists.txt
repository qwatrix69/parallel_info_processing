cmake_minimum_required(VERSION 3.10)
project(MPI_Lab VERSION 1.0 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(${MPI_C_INCLUDE_PATH})

add_executable(lab2 lab2.c)
target_link_libraries(lab2 ${MPI_C_LIBRARIES})
target_compile_options(lab2 PRIVATE ${MPI_C_COMPILE_FLAGS})
