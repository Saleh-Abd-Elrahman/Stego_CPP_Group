# Specify the minimum CMake version and project metadata
cmake_minimum_required(VERSION 3.20) # Minimum version of CMake required
project(CPPSTEGANOGRAPHY VERSION 1.0) # Project name and version

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(COPY ${CMAKE_SOURCE_DIR}/external DESTINATION ${CMAKE_BINARY_DIR})

# We place the executables in the bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# We place the libraries in the lib directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Enable Objective-C language for MacOS if needed
if(APPLE)
    enable_language(OBJC)
    set(CMAKE_OBJC_FLAGS "${CMAKE_C_FLAGS}")
endif()

# Include project directories 
include_directories(include)
include_directories(src)

# Find and include GLFW
find_package(GLFW3 REQUIRED)

# Find and include IMGUI
find_package(imgui REQUIRED)

# Add subdirectories for modular builds and testing
add_subdirectory(src)
enable_testing()
add_subdirectory(test)
 

# Link libraries

