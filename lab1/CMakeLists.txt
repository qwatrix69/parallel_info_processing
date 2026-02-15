cmake_minimum_required(VERSION 3.10)
project(MPI_Lab VERSION 1.0 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(${MPI_C_INCLUDE_PATH})

add_executable(lab1 lab1.c)
target_link_libraries(lab1 ${MPI_C_LIBRARIES})
target_compile_options(lab1 PRIVATE ${MPI_C_COMPILE_FLAGS})


add_executable(lab1v lab1v.c)
target_link_libraries(lab1v ${MPI_C_LIBRARIES})
target_compile_options(lab1v PRIVATE ${MPI_C_COMPILE_FLAGS})
