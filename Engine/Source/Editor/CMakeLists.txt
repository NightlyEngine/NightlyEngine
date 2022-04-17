cmake_minimum_required(VERSION 3.20)
project(NightlyEditor)

set(CMAKE_CXX_STANDARD 20)

add_executable(NightlyEditor EditorApp.cpp)

target_precompile_headers(NightlyEditor PUBLIC <iostream>)

target_link_libraries(NightlyEditor NightlyEngine)