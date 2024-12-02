#include "encoder.h"
#include "decoder.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Paths for encoding and decoding a text file
    const std::string inputImageText = "../images/input.png";
    const std::string outputImageText = "../images/output.png";
    const std::string inputTextFile = "../text/message.txt";
    const std::string outputTextFile = "../text/decoded_message.txt";

    // Paths for encoding and decoding a Bash script file
    const std::string inputImageScript = "../images/input_script.png";
    const std::string outputImageScript = "../images/output_script.png";
    const std::string inputScriptFile = "../scripts/script.sh";
    const std::string outputScriptFile = "../scripts/decoded_script.sh";

    // Encode the text file into the PNG image
    if (!encodeFileInPNG(inputImageText, outputImageText, inputTextFile)) {
        std::cerr << "Failed to encode the text file into the image." << std::endl;
        return 1;
    }

    // Decode the text message from the PNG image and save it to a file
    if (!decodeFileFromPNG(outputImageText, outputTextFile)) {
        std::cerr << "Failed to decode the text message from the image." << std::endl;
        return 1;
    }

    std::cout << "Successfully encoded and decoded the text file." << std::endl;

    // Encode the Bash script file into the PNG image
    if (!encodeFileInPNG(inputImageScript, outputImageScript, inputScriptFile)) {
        std::cerr << "Failed to encode the Bash script into the image." << std::endl;
        return 1;
    }

    // Prompt the user whether to decode and execute the script
    std::cout << "Do you want to decode and execute the Bash script from the image? (y/n): ";
    char choice;
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        if (!decodeAndExecuteScript(outputImageScript, outputScriptFile)) {
            std::cerr << "Failed to decode and execute the Bash script from the image." << std::endl;
            return 1;
        }
        std::cout << "Successfully decoded and executed the Bash script file." << std::endl;
    } else {
        std::cout << "Skipping script execution as per user request." << std::endl;
    }

    return 0;
}
