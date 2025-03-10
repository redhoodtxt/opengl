cmake_minimum_required(VERSION 3.10.0)
project(shaders)

# Ensure we use at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Disable GLFW's unnecessary components
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add external dependencies
add_subdirectory(external/glfw-3.4)

# Find OpenGL
find_package(OpenGL REQUIRED)


# Source files for the project
set(SHADERS_SOURCES
    external/glad/src/glad.c
    src/main.cpp
    src/shader.cpp
)

# Header files (optional, not necessary for compilation)
set(SHADERS_HEADERS
    external/glad/include/glad/glad.h
    ./include/shaders.h
)

# Create executable for the CHIP-8 emulator
add_executable(shaders ${SHADERS_SOURCES} ${SHADERS_HEADERS})

# Include necessary directories
target_include_directories(shaders PUBLIC
    external/glfw-3.4/include
    external/glad/include
    ./include
)


# Link the required libraries
target_link_libraries(shaders PRIVATE
    glfw
    # glad i dont need to fucking link this shit here 
    OpenGL::GL
)

# Ensure the executable is placed inside the 'bin' directory in the build folder
set_target_properties(shaders PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
