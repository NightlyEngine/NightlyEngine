cmake_minimum_required(VERSION 3.16)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(NightlyEngine SHARED Core/Application.h Core/Application.cpp Core/Core.h Core/Engine.h)

# Get rid of an additional <CONFIGURATION> directory
set_target_properties(NightlyEngine PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Engine/Binaries/Debug"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Engine/Binaries/Release")

target_precompile_headers(NightlyEngine PUBLIC
        <iostream>
        <string>
        <vector>
        )

target_link_libraries(NightlyEngine glfw)
