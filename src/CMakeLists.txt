# Find the libsndfile library
find_package(SndFile REQUIRED)

# Add the executable
add_executable(steganography main.cpp Encoder.cpp Decoder.cpp)

# Link the libsndfile library
target_link_libraries(steganography PRIVATE SndFile::sndfile)
