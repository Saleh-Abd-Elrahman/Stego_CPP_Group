#include "encoder.h"
#include "decoder.h"
#include <iostream>
#include <fstream>

int main() {
    const std::string inputImage = "../images/input.png";
    const std::string outputImage = "../images/output.png";
    const std::string inputTextFile = "../text/message.txt";
    const std::string outputTextFile = "../text/decoded_message.txt";

    // Encode the text file into the PNG image
    if (!encodeFileInPNG(inputImage, outputImage, inputTextFile)) {
        std::cerr << "Failed to encode the file into the image." << std::endl;
        return 1;
    }

    // Decode the message from the PNG image and save it to a file
    if (!decodeFileFromPNG(outputImage, outputTextFile)) {
        std::cerr << "Failed to decode the message from the image." << std::endl;
        return 1;
    }

    std::cout << "Successfully encoded and decoded the text file." << std::endl;
    return 0;
}
