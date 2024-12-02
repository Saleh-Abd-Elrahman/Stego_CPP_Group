#include "encoder.h"
#include "decoder.h"
#include <iostream>
#include <string>


int main() {
    std::string inputImagePath = "../images/input.png";
    std::string outputImagePath = "../images/output.png";
    std::string message = "Hello, this is a hidden message!";

    // Encode the message into the image
    std::cout << "Encoding message into PNG..." << std::endl;
    encodeMessageInPNG(inputImagePath, outputImagePath, message);

    // Decode the message from the encoded image
    std::cout << "Decoding message from PNG..." << std::endl;
    std::string decodedMessage = decodeMessageFromPNG(outputImagePath);

    // Check if the decoded message matches the original message
    if (decodedMessage == message) {
        std::cout << "Success! Decoded message matches the original message." << std::endl;
    } else {
        std::cerr << "Error: Decoded message does not match the original message." << std::endl;
    }

    return 0;
}