cmake_minimum_required(VERSION 3.16)
project(NightlyEditor)

set(CMAKE_CXX_STANDARD 20)

add_executable(NightlyEditor
        EditorApp.cpp
        EditorUI.cpp
        EditorUI.h
        Windows/EditorWindow.cpp
        Windows/EditorWindow.h
        Windows/DockspaceWindow.cpp
        Windows/DockspaceWindow.h
        Windows/WorldWindow.cpp
        Windows/WorldWindow.h)

# Set rpath relative to editor app
if (NOT MSVC)
    set_target_properties(NightlyEditor PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
endif ()

# Precompiled headers
target_precompile_headers(NightlyEditor PUBLIC ../Runtime/Core/PCH.h)

# Link editor with submodules
target_link_libraries(NightlyEditor NightlyEngine)
target_link_libraries(NightlyEditor imgui)
