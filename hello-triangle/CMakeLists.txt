cmake_minimum_required(VERSION 3.10.0)
project(hello-triangle)

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
set(TRIANGLE_SOURCES
    external/glad/src/glad.c
    src/main.cpp
)

# Header files (optional, not necessary for compilation)
set(TRIANGLE_HEADERS
    external/glad/include/glad/glad.h
)

# Create executable for the CHIP-8 emulator
add_executable(hello-triangle ${TRIANGLE_SOURCES} ${TRIANGLE_HEADERS})

# Include necessary directories
target_include_directories(hello-triangle PUBLIC
    external/glfw-3.4/include
    external/glad/include
)


# Link the required libraries
target_link_libraries(hello-triangle PRIVATE
    glfw
    # glad i dont need to fucking link this shit here 
    OpenGL::GL
)

# Ensure the executable is placed inside the 'bin' directory in the build folder
set_target_properties(hello-triangle PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
