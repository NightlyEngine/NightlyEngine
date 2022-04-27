cmake_minimum_required(VERSION 3.16)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

# Disable unnecessary build targets
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Set MSVC compiler flags
if (MSVC)
    add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
    )
endif ()

if (MSVC)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Engine/Binaries")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Engine/Binaries")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Engine/Binaries")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NightlyEditor)
else ()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Engine/Binaries/${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Engine/Binaries/${CMAKE_BUILD_TYPE}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Engine/Binaries/${CMAKE_BUILD_TYPE}")
endif ()

# Get rid of an additional <CONFIGURATION> directory
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/Engine/Binaries/${CMAKE_BUILD_TYPE}")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/Engine/Binaries/${CMAKE_BUILD_TYPE}")

# Include the source directory
include_directories("${PROJECT_SOURCE_DIR}/Engine/Source")

# Add third party dependencies
add_subdirectory(Engine/Source/ThirdParty/glfw)
add_subdirectory(Engine/Source/ThirdParty/glad)
add_subdirectory(Engine/Source/ThirdParty/glm)

# Add Nightly modules
add_subdirectory(Engine/Source/Runtime)
add_subdirectory(Engine/Source/Editor)
add_subdirectory(Engine/Source/Plugins)

# Add preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE NL_EXPORT_DLL)

# Set third party include paths
target_include_directories(${PROJECT_NAME} PUBLIC "Engine/Source/ThirdParty/glfw/include")
target_include_directories(${PROJECT_NAME} PUBLIC "Engine/Source/ThirdParty/glad/include")
target_include_directories(${PROJECT_NAME} PUBLIC "Engine/Source/ThirdParty/glm")

# Set engine include paths
target_include_directories(${PROJECT_NAME} PUBLIC "Engine/Source/Runtime")
target_include_directories(${PROJECT_NAME} PUBLIC "Engine/Source/Editor")
target_include_directories(${PROJECT_NAME} PUBLIC "Engine/Source/Plugins")
