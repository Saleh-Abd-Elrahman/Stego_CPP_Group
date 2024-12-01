# Find and include PNG
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Compile the main library first (encoder and decoder)
add_library(main_library encoder.cpp decoder.cpp)

# Include directories for the main library
target_include_directories(main_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Compile the executable (my_executable)
add_executable(my_executable main.cpp)

# Set the output directory for the executable
set_target_properties(my_executable PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/project)

# Link the main library and PNG to the executable
target_link_libraries(my_executable main_library ${JPEG_LIBRARIES})
target_link_libraries(main_library ${JPEG_LIBRARIES}) 