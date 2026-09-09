cmake_minimum_required(VERSION 3.28)
project(Apogee)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # fail instead of hiding the issue
set(CMAKE_CXX_EXTENSIONS OFF)         # use pure C++20, no GNU extensions

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_executable(Apogee main.cpp
        gl_loader.cpp)
target_link_libraries(Apogee PRIVATE glfw OpenGL::GL)
