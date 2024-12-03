#include "encoder_PNG.h"
#include "decoder_PNG.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string inputImagePath = "../external/input.png";
    std::string outputImagePath = "../external/output.png";
    std::string message = "Hello, Stego!";
    std::string passwordEncode = "Passw0rd"; // Must be 8 characters
    std::string passwordDecode = "Passw0rd"; // Must be 8 characters
    
    // Encode the message into the image with the provided password
    encodeMessageInPNG(inputImagePath, outputImagePath, message, passwordEncode);
    std::cout << "Message encoded successfully with password: " << passwordEncode << std::endl;
    
    // Decode the message from the image with the provided password
    std::string decodedMessage = decodeMessageFromPNG(outputImagePath, passwordDecode);
    if (!decodedMessage.empty()) {
        std::cout << "Decoded Message: " << decodedMessage << std::endl;
    } else {
        std::cerr << "Failed to decode the message. Possible incorrect password or corrupted data." << std::endl;
    }
    
    return 0;
}
