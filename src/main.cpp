#include "encoder.h"
#include "decoder.h"
#include <iostream>
#include <fstream>

int main() {
    const std::string inputImage = "../images/input.jpg";
    const std::string outputImage = "../images/output.jpg";
    const std::string inputTextFile = "../text/message.txt";
    const std::string outputTextFile = "../text/decoded_message.txt";

    // Encode the text file into the JPEG image
    if (!encodeFileInJPEG(inputImage, outputImage, inputTextFile)) {
        std::cerr << "Failed to encode the file into the image." << std::endl;
        return 1;
    }

    // Decode the message from the JPEG image and save it to a file
    if (!decodeFileFromJPEG(outputImage, outputTextFile)) {
        std::cerr << "Failed to decode the message from the image." << std::endl;
        return 1;
    }

    std::cout << "Successfully encoded and decoded the text file." << std::endl;
    return 0;
}
