#include "encoder.h"
#include "decoder.h"
#include <iostream>

int main() {
    const std::string inputImage = "../images/input.png";
    const std::string outputImage = "../images/output.png";
    const std::string message = "Secret message";

    encodeMessageInPNG(inputImage, outputImage, message);

    std::string decodedMessage = decodeMessageFromPNG(outputImage);
    std::cout << "Decoded Message: " << decodedMessage << std::endl;

    return 0;
}
