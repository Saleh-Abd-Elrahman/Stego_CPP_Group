cmake_minimum_required(VERSION 3.10)
project(CPPSTEGANOGRAPHY)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# MacOS specific settings
enable_language(OBJC)
set(CMAKE_OBJC_FLAGS "${CMAKE_C_FLAGS}")

# Include project directories 
include_directories(include)
include_directories(src)

# Find and include GLFW
find_package(GLFW3 REQUIRED)

# Find and include IMGUI
find_package(imgui REQUIRED)

# Specify source files
set(SOURCES
    src/main.cpp
    src/encode.cpp
    src/decode.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} imgui::imgui SDL2 glfw)