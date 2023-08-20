cmake_minimum_required(VERSION 3.1)
project(minecraftpe)

add_subdirectory(openal-soft)
add_subdirectory(glad/cmake)

glad_add_library(glad_egl_15_gl1_15 REPRODUCIBLE LOADER API egl=1.5 gl:core=1.5)

include_directories(glfw/include)
include_directories(stb)
add_subdirectory(glfw)
add_subdirectory(minecraftpe)