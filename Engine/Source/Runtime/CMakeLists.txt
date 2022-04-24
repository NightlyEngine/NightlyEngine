cmake_minimum_required(VERSION 3.16)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(NightlyEngine SHARED Core/Application.h Core/Core.h Core/Engine.h Core/Log.h Core/Log.cpp Core/Engine.cpp Platform/PlatformDetection.h Platform/WindowsPlatform.h Platform/WindowsPlatform.cpp Core/EngineTime.h Core/EngineTime.cpp Core/EngineAPI.h Core/Window.h Core/Window.cpp Core/WindowManager.h Core/WindowManager.cpp Renderer/Renderer.h Renderer/Renderer.cpp Core/MemoryManager.h Core/ITrackable.h Core/MemoryManager.cpp)

# Get rid of an additional <CONFIGURATION> directory
set_target_properties(NightlyEngine PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Engine/Binaries/Debug"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Engine/Binaries/Release")

target_precompile_headers(NightlyEngine PUBLIC
        <iostream>
        <string>
        <vector>
        <sstream>
        <memory>
        )

target_link_libraries(NightlyEngine glad)
target_link_libraries(NightlyEngine glfw)
target_link_libraries(NightlyEngine glm)
