cmake_minimum_required(VERSION 3.16)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(NightlyEngine SHARED
        Core/Application.h
        Core/Core.h
        Core/Engine.h
        Core/Log.h
        Core/Log.cpp
        Core/Engine.cpp
        Core/EngineTime.h
        Core/EngineTime.cpp
        Core/EngineAPI.h
        Core/Window.h
        Core/Window.cpp
        Core/WindowManager.h
        Core/WindowManager.cpp
        Core/MemoryManager.h
        Core/MemoryManager.cpp
        Core/ITrackable.h
        Core/Plugin.h
        Core/PluginManager.h
        Core/PluginManager.cpp
        Core/Math.h
        Core/Math.cpp
        Platform/PlatformDetection.h
        Platform/WindowsPlatform.h
        Platform/WindowsPlatform.cpp
        Renderer/Renderer.h
        Renderer/Renderer.cpp
        Renderer/Shader.h
        Renderer/Shader.cpp
        )

target_precompile_headers(NightlyEngine PUBLIC
        <iostream>
        <string>
        <vector>
        <sstream>
        <memory>
        <fstream>
        )

target_link_libraries(NightlyEngine glad)
target_link_libraries(NightlyEngine glfw)
target_link_libraries(NightlyEngine glm)
