cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 11)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Set include directories for MacOS
include_directories(
    /opt/homebrew/include
    /opt/homebrew/opt/glfw/include
)

# Set library directories
link_directories(
    /opt/homebrew/lib
)

# Find required packages
find_package(OpenGL REQUIRED)

# Add the executable
add_executable(OpenGLProject main.cpp)

# Link libraries
target_link_libraries(OpenGLProject
    ${OPENGL_LIBRARIES}
    glfw
    GLEW
)
