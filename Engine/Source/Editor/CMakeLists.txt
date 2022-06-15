cmake_minimum_required(VERSION 3.16)
project(NightlyEditor)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_executable(NightlyEditor
        EditorApp.cpp
        EditorUI.cpp
        EditorUI.h
        UIBackend.cpp
        UIBackend.h)

# Set rpath relative to editor app
if (NOT MSVC)
    set_target_properties(NightlyEditor PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
endif ()

# Precompiled headers
target_precompile_headers(NightlyEditor PUBLIC ../Runtime/Core/PCH.h)

# Link editor with submodules
target_link_libraries(NightlyEditor NightlyEngine)
target_link_libraries(NightlyEditor imgui)
