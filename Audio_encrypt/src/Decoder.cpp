#include "Steganography.h"
#include <sndfile.hh>
#include <vector>
#include <bitset>
#include <iostream>

std::string Steganography::decode(const std::string& inputWav) {
    // Open input WAV file
    SndfileHandle inputFile(inputWav);
    if (inputFile.error()) {
        std::cerr << "Error reading input WAV file: " << inputFile.strError() << std::endl;
        return "";
    }

    // Read audio samples
    std::vector<short> samples(inputFile.frames() * inputFile.channels());
    inputFile.read(&samples[0], samples.size());

    // Extract LSBs from audio samples
    std::string binaryData;
    for (size_t i = 0; i < samples.size(); ++i) {
        binaryData += (samples[i] & 1) ? '1' : '0';
    }

    // Convert binary data to message
    std::string message = binaryToMessage(binaryData);
    return message;
}

std::string Steganography::binaryToMessage(const std::string& binaryData) {
    std::string message;
    for (size_t i = 0; i + 7 < binaryData.size(); i += 8) {
        std::string byteString = binaryData.substr(i, 8);
        if (byteString == "00000000") {
            // Null character detected, end of message
            break;
        }
        char c = static_cast<char>(std::bitset<8>(byteString).to_ulong());
        message += c;
    }
    return message;
}
