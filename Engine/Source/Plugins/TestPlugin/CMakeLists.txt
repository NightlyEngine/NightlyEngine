cmake_minimum_required(VERSION 3.16)
project(NightlyPlugin_Test)

set(CMAKE_CXX_STANDARD 20)

add_library(NightlyPlugin_Test SHARED TestPlugin.h TestPlugin.cpp)

# Get rid of an additional <CONFIGURATION> directory
#[[set_target_properties(NightlyPlugin_Test PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Engine/Binaries/Debug"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Engine/Binaries/Release")]]

target_precompile_headers(NightlyPlugin_Test PUBLIC <iostream> <string> <vector> <sstream>)

target_link_libraries(NightlyPlugin_Test NightlyEngine)

#TODO: Create CMakeLists.txt automatically