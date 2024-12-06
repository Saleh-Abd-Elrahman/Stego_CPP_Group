# Find and include PNG
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

# Find and include SndFile
find_package(SndFile REQUIRED)
include_directories(${SNDFILE_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

# Compile the main library first (encoder and decoder)
add_library(main_library encoder_PNG.cpp decoder_PNG.cpp Encoder_WAV.cpp Decoder_WAV.cpp vigenere.cpp operations_PNG.cpp encode.cpp decode.cpp file_utils.cpp)

# Include directories for the main library
target_include_directories(main_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(my_executable main.cpp)

# Set the output directory for the executable
set_target_properties(my_executable PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/project)

# Link the main library and PNG to the executable
target_link_libraries(my_executable main_library ${PNG_LIBRARIES})
target_link_libraries(main_library ${PNG_LIBRARIES} ${SNDFILE_LIBRARIES}) 
target_link_libraries(main_library imgui::imgui SDL2 glfw)
