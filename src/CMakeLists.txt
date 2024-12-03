# Find the libsndfile library
find_package(SndFile REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${SNDFILE_INCLUDE_DIR})

# Compile the main library first (encoder and decoder)
add_library(main_library Encoder_WAV.cpp Decoder_WAV.cpp vigenere_WAV.cpp)

# Include directories for the main library
target_include_directories(main_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Compile the executable (my_executable)
add_executable(WAV main.cpp)

# Set the output directory for the executable
set_target_properties(WAV PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/project)

target_link_libraries(WAV main_library ${SNDFILE_LIBRARIES})
target_link_libraries(main_library ${SNDFILE_LIBRARIES}) 
