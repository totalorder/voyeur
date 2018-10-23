cmake_minimum_required(VERSION 3.12)
project(ctest1 C)

set(CMAKE_C_STANDARD 11)
add_executable(main main.c common.c common.h)

add_executable(child child.c common.c common.h)