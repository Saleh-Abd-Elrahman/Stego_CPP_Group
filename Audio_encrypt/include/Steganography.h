#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <string>

class Steganography {
public:
    // Encodes a message into a WAV file
    static bool encode(const std::string& inputWav, const std::string& outputWav, const std::string& message);

    // Decodes a message from a WAV file
    static std::string decode(const std::string& inputWav);

private:
    // Helper functions
    static std::string messageToBinary(const std::string& message);
    static std::string binaryToMessage(const std::string& binaryData);
};

#endif // STEGANOGRAPHY_H
