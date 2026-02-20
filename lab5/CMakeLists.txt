cmake_minimum_required(VERSION 3.10)
project(OpenMP_Lab VERSION 1.0 LANGUAGES C)

find_package(OpenMP REQUIRED)

add_executable(lab5 lab5.c)

target_link_libraries(lab5 PRIVATE OpenMP::OpenMP_C)
target_compile_options(lab5 PRIVATE ${OpenMP_C_FLAGS})


