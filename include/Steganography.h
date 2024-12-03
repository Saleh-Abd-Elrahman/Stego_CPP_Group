#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <string>

class Steganography {
public:
    // Encodes a message into a WAV file with password protection
    static bool encodeMessage(const std::string& inputWav, 
                              const std::string& outputWav, 
                              const std::string& message, 
                              const std::string& password);

    // Decodes a message from a WAV file with password verification
    static std::string decodeMessage(const std::string& inputWav, 
                                     const std::string& password);

    // Encodes a file (text, PNG, Bash script) into a WAV file with password protection
    static bool encodeFile(const std::string& inputWav, 
                           const std::string& outputWav, 
                           const std::string& inputFilePath, 
                           const std::string& password);

    // Decodes a file from a WAV file with password verification
    static bool decodeFile(const std::string& inputWav, 
                           const std::string& outputFilePath, 
                           const std::string& password);

private:
    // Helper functions
    static std::string messageToBinary(const std::string& message);
    static std::string binaryToMessage(const std::string& binaryData);
    static std::string fileToBinary(const std::string& filePath);
    static bool binaryToFile(const std::string& binaryData, const std::string& outputFilePath);
    static bool eraseHiddenData(const std::string& wavPath);
};

#endif // STEGANOGRAPHY_H
