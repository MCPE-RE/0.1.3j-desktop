cmake_minimum_required (VERSION 3.1)

project(minecraftpe-core)

file(GLOB_RECURSE MC_SOURCES ../0.1.3j-core/src/*.cpp)

file(GLOB_RECURSE SOURCE_FILES ./src/*.cpp)

add_executable(minecraftpe ../stb/stb_vorbis.c ${SOURCE_FILES} ${MC_SOURCES})
target_compile_definitions(minecraftpe PRIVATE USE_DESKTOP_GL)
target_include_directories(minecraftpe PUBLIC include/)
target_link_libraries(minecraftpe ${CMAKE_DL_LIBS} glad_egl_15_gl1_15 glfw OpenAL)