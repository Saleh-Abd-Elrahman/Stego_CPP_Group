#include "Steganography.h"
#include <sndfile.hh>
#include <vector>
#include <bitset>
#include <iostream>

bool Steganography::encode(const std::string& inputWav, const std::string& outputWav, const std::string& message) {
    // Open input WAV file
    SndfileHandle inputFile(inputWav);
    if (inputFile.error()) {
        std::cerr << "Error reading input WAV file: " << inputFile.strError() << std::endl;
        return false;
    }

    // Read audio samples
    std::vector<short> samples(inputFile.frames() * inputFile.channels());
    inputFile.read(&samples[0], samples.size());

    // Convert message to binary
    std::string binaryMessage = messageToBinary(message);

    // Check if the message can fit in the audio file
    if (binaryMessage.size() > samples.size()) {
        std::cerr << "Message is too long to encode in this audio file." << std::endl;
        return false;
    }

    // Encode message into LSB of audio samples
    for (size_t i = 0; i < binaryMessage.size(); ++i) {
        samples[i] &= ~1; // Clear LSB
        samples[i] |= (binaryMessage[i] == '1') ? 1 : 0; // Set LSB
    }

    // Write modified samples to output WAV file
    SndfileHandle outputFile(outputWav, SFM_WRITE, inputFile.format(), inputFile.channels(), inputFile.samplerate());
    if (outputFile.error()) {
        std::cerr << "Error writing output WAV file: " << outputFile.strError() << std::endl;
        return false;
    }
    outputFile.write(&samples[0], samples.size());

    return true;
}

std::string Steganography::messageToBinary(const std::string& message) {
    std::string binaryString;
    for (char c : message) {
        std::bitset<8> bits(c);
        binaryString += bits.to_string();
    }
    // Append a delimiter to indicate end of message
    binaryString += "00000000"; // Null character as a delimiter
    return binaryString;
}
