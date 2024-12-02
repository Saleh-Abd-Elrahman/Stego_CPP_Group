#include "Steganography.h"
#include <iostream>

int main() {
    std::string inputWav = "../data/input.wav";
    std::string outputWav = "../data/output.wav";
    std::string message = "Hello, World!";

    // Encode the message
    if (Steganography::encode(inputWav, outputWav, message)) {
        std::cout << "Message encoded successfully into " << outputWav << std::endl;
    } else {
        std::cerr << "Failed to encode message." << std::endl;
        return 1;
    }

    // Decode the message
    std::string decodedMessage = Steganography::decode(outputWav);
    if (!decodedMessage.empty()) {
        std::cout << "Decoded message: " << decodedMessage << std::endl;
    } else {
        std::cerr << "Failed to decode message." << std::endl;
        return 1;
    }

    return 0;
}
