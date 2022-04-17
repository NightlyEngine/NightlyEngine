cmake_minimum_required(VERSION 3.22)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

# Disable unnecessary build targets
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories("${PROJECT_SOURCE_DIR}/Engine/Source")

# Add third party dependencies
add_subdirectory(Engine/Source/ThirdParty/glfw)

# Add Nightly modules
add_subdirectory(Engine/Source/Runtime)
add_subdirectory(Engine/Source/Editor)