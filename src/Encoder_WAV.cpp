#include "WAV.h"
#include "vigenere_WAV.h"
#include <sndfile.hh>
#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>

bool Steganography::encodeMessageInWAV(const std::string& inputWav, 
                                  const std::string& outputWav, 
                                  const std::string& message, 
                                  const std::string& password) {
    // Fixed Vigenère key for password scrambling
    const std::string vigenereKey = "VIGENEREK"; // Must be at least 8 characters

    // Ensure password is exactly 8 characters
    if (password.size() != 8) {
        std::cerr << "Error: Password must be exactly 8 characters long." << std::endl;
        return false;
    }

    // Scramble the password using Vigenère Cipher
    std::string scrambledPassword = encryptVigenere(password, vigenereKey);

    // Combine scrambled password with the message
    // Format: [32 bits length][64 bits scrambled password][message bits]
    uint32_t messageLength = message.size();
    std::string binaryLength;
    for (int i = 31; i >= 0; i--) {
        binaryLength += ((messageLength >> i) & 1) ? '1' : '0';
    }

    // Convert scrambled password to binary
    std::string binaryPassword;
    for (char c : scrambledPassword) {
        for (int i = 7; i >= 0; i--) {
            binaryPassword += ((c >> i) & 1) ? '1' : '0';
        }
    }

    // Convert the message to binary
    std::string binaryMessage = messageToBinary(message);

    // Concatenate all binary data
    std::string fullBinaryData = binaryLength + binaryPassword + binaryMessage;

    // Open input WAV file
    SndfileHandle inputFile(inputWav);
    if (inputFile.error()) {
        std::cerr << "Error reading input WAV file: " << inputFile.strError() << std::endl;
        return false;
    }

    // Read audio samples
    std::vector<short> samples(inputFile.frames() * inputFile.channels());
    inputFile.read(&samples[0], samples.size());

    // Check if the message can fit in the audio file
    if (fullBinaryData.size() > samples.size()) {
        std::cerr << "Error: The audio file is too small to hold the entire data (length, password, message)." << std::endl;
        return false;
    }

    // Encode message into LSB of audio samples
    for (size_t i = 0; i < fullBinaryData.size(); ++i) {
        samples[i] &= ~1; // Clear LSB
        samples[i] |= (fullBinaryData[i] == '1') ? 1 : 0; // Set LSB
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

bool Steganography::encodeFileInWAV(const std::string& inputWav, 
                               const std::string& outputWav, 
                               const std::string& inputFilePath, 
                               const std::string& password) {
    // Fixed Vigenère key for password scrambling
    const std::string vigenereKey = "VIGENEREK"; // Must be at least 8 characters

    // Ensure password is exactly 8 characters
    if (password.size() != 8) {
        std::cerr << "Error: Password must be exactly 8 characters long." << std::endl;
        return false;
    }

    // Read the input file as binary
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Unable to open input file " << inputFilePath << std::endl;
        return false;
    }
    std::string fileData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Scramble the password using Vigenère Cipher
    std::string scrambledPassword = encryptVigenere(password, vigenereKey);

    // Combine scrambled password with the file data
    // Format: [32 bits length][64 bits scrambled password][file data bits]
    uint32_t fileLength = fileData.size();
    std::string binaryLength;
    for (int i = 31; i >= 0; i--) {
        binaryLength += ((fileLength >> i) & 1) ? '1' : '0';
    }

    // Convert scrambled password to binary
    std::string binaryPassword;
    for (char c : scrambledPassword) {
        for (int i = 7; i >= 0; i--) {
            binaryPassword += ((c >> i) & 1) ? '1' : '0';
        }
    }

    // Convert the file data to binary
    std::string binaryFileData = fileToBinary(fileData);

    // Concatenate all binary data
    std::string fullBinaryData = binaryLength + binaryPassword + binaryFileData;

    // Open input WAV file
    SndfileHandle wavInputFile(inputWav);
    if (wavInputFile.error()) {
        std::cerr << "Error reading input WAV file: " << wavInputFile.strError() << std::endl;
        return false;
    }

    // Read audio samples
    std::vector<short> samples(wavInputFile.frames() * wavInputFile.channels());
    wavInputFile.read(&samples[0], samples.size());

    // Check if the file can fit in the audio file
    if (fullBinaryData.size() > samples.size()) {
        std::cerr << "Error: The audio file is too small to hold the entire data (length, password, file)." << std::endl;
        return false;
    }

    // Encode file data into LSB of audio samples
    for (size_t i = 0; i < fullBinaryData.size(); ++i) {
        samples[i] &= ~1; // Clear LSB
        samples[i] |= (fullBinaryData[i] == '1') ? 1 : 0; // Set LSB
    }

    // Write modified samples to output WAV file
    SndfileHandle wavOutputFile(outputWav, SFM_WRITE, wavInputFile.format(), wavInputFile.channels(), wavInputFile.samplerate());
    if (wavOutputFile.error()) {
        std::cerr << "Error writing output WAV file: " << wavOutputFile.strError() << std::endl;
        return false;
    }
    wavOutputFile.write(&samples[0], samples.size());

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

std::string Steganography::fileToBinary(const std::string& fileData) {
    std::string binaryString;
    for (char c : fileData) {
        std::bitset<8> bits(c);
        binaryString += bits.to_string();
    }
    return binaryString;
}
