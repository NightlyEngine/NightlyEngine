cmake_minimum_required(VERSION 3.22)
project(NightlyEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(NightlyEngine SHARED Engine/Source/Runtime/Core/library.cpp)
