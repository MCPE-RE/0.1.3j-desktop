cmake_minimum_required (VERSION 3.1)

project(minecraftpe-core)

#find_package(OpenAL REQUIRED)

file(GLOB_RECURSE MC_SOURCES ../0.1.3j-core/src/*.cpp)

set(SOURCE_FILES
    src/Gvs.cpp
    src/AppPlatform_linux.cpp
    src/Main.cpp
)

add_executable(minecraftpe ${SOURCE_FILES} ${MC_SOURCES})
target_compile_definitions(minecraftpe PRIVATE USE_DESKTOP_GL)
target_include_directories(minecraftpe PUBLIC include/)
target_link_libraries(minecraftpe ${CMAKE_DL_LIBS} soil2 glad_egl_15_gl1_15 glfw)