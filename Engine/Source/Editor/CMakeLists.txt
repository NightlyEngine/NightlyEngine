cmake_minimum_required(VERSION 3.16)
project(NightlyEditor)

set(CMAKE_CXX_STANDARD 20)

add_executable(NightlyEditor EditorApp.cpp)

# Set rpath relative to editor app
if (NOT MSVC)
    set_target_properties(NightlyEditor PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
endif ()

# Precompiled headers
target_precompile_headers(NightlyEditor PUBLIC <iostream>)

# Link editor with engine
target_link_libraries(NightlyEditor NightlyEngine)
