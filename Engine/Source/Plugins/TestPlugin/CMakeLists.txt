cmake_minimum_required(VERSION 3.16)
project(NightlyPlugin_Test)

set(CMAKE_CXX_STANDARD 20)

add_library(NightlyPlugin_Test SHARED TestPlugin.h TestPlugin.cpp)

target_precompile_headers(NightlyPlugin_Test PUBLIC <iostream> <string> <vector> <sstream>)

target_link_libraries(NightlyPlugin_Test NightlyEngine)