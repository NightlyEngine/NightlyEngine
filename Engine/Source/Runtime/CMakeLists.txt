cmake_minimum_required(VERSION 3.20)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(NL_EXPORT)

add_library(NightlyEngine SHARED Core/Application.h Core/Application.cpp Core/Core.h Core/Engine.h)

target_precompile_headers(NightlyEngine PUBLIC
        <iostream>
        <string>
        <vector>
        )

target_link_libraries(NightlyEngine glfw)
