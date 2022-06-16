cmake_minimum_required(VERSION 3.16)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(NightlyEngine SHARED
        Core/Application.h
        Core/Core.h
        Core/Log.h
        Core/Log.cpp
        Core/Engine.h
        Core/Engine.cpp
        Core/EngineTime.h
        Core/EngineTime.cpp
        Core/EngineAPI.h
        Core/Event/Event.h
        Core/Event/EventSystem.cpp
        Core/Event/EventSystem.h
        Core/Event/WindowEvents.h
        Core/Event/WindowEvents.cpp
        Core/Event/InputSystem.cpp
        Core/Event/InputSystem.h
        Core/Event/InputDefinitions.h
        Core/Event/InputEvents.h
        Core/Event/InputEvents.cpp
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
        Core/UUID.h
        Core/UUID.cpp
        Platform/PlatformDetection.h
        Platform/WindowsPlatform.h
        Platform/WindowsPlatform.cpp
        Renderer/Renderer.h
        Renderer/Renderer.cpp
        Renderer/Shader.h
        Renderer/Shader.cpp
        Renderer/Framebuffer.h
        Renderer/Framebuffer.cpp
        Renderer/ScreenPlane.h
        Renderer/ScreenPlane.cpp
        World/Entity.h
        World/Entity.cpp
        World/World.h
        World/World.cpp
        World/WorldManager.h
        World/WorldManager.cpp
        World/Components/Component.h
        World/Components/Component.cpp
        World/Components/TransformComponent.h
        World/Components/TransformComponent.cpp
        World/Components/MeshComponent.h
        World/Components/MeshComponent.cpp
        World/Components/CameraComponent.h
        World/Components/CameraComponent.cpp
        )

target_precompile_headers(NightlyEngine PUBLIC Core/PCH.h)

target_link_libraries(NightlyEngine PUBLIC glad glfw glm ${CMAKE_DL_LIBS})

