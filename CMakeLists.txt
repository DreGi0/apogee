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

add_executable(Apogee
        src/main.cpp
        src/gl_loader.cpp
        src/shader.cpp
        src/paths.cpp
)

target_compile_definitions(Apogee PRIVATE $<$<CONFIG:Debug>:APOGEE_DEBUG>)
target_link_libraries(Apogee PRIVATE glfw OpenGL::GL)

add_custom_command(TARGET Apogee POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Apogee>/assets
        COMMENT "Copying assets next to the executable")
