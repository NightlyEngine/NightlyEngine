cmake_minimum_required(VERSION 3.0)
project(glad)

add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)

# Add compiler flags on Linux
if (UNIX AND NOT APPLE)
    target_compile_options(glad PRIVATE "-fPIC")
endif ()