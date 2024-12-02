#include "encoder.h"
#include "decoder.h"
#include <iostream>
#include <fstream>
#include <string>

// Function Prototypes
void encodeDirectMessage();
void encodeFile();
void decodeMessage();
void decodeFile();

int main() {
    int choice = 0;

    while (true) {
        std::cout << "\n=== Steganography Application ===\n";
        std::cout << "1. Encode a Direct Message into PNG\n";
        std::cout << "2. Encode a File into PNG\n";
        std::cout << "3. Decode a Message from PNG\n";
        std::cout << "4. Decode a File from PNG\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;

        // Clear input buffer to handle any leftover input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                encodeDirectMessage();
                break;
            case 2:
                encodeFile();
                break;
            case 3:
                decodeMessage();
                break;
            case 4:
                decodeFile();
                break;
            case 5:
                std::cout << "Exiting the application. Goodbye!\n";
                return 0;
            default:
                std::cerr << "Invalid choice. Please select a number between 1 and 5.\n";
        }
    }

    return 0;
}

// Function Definitions

void encodeDirectMessage() {
    std::string inputImagePath, outputImagePath, message, password;

    std::cout << "\n--- Encode Direct Message ---\n";

    // Get input image path
    std::cout << "Enter the path to the input PNG image: ";
    std::getline(std::cin, inputImagePath);

    // Get output image path
    std::cout << "Enter the desired path for the output PNG image: ";
    std::getline(std::cin, outputImagePath);

    // Get the message to encode
    std::cout << "Enter the message to hide: ";
    std::getline(std::cin, message);

    // Get password for encoding
    while (true) {
        std::cout << "Enter an 8-character password for encoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Encode the message into the image with the provided password
    encodeMessageInPNG(inputImagePath, outputImagePath, message, password);
    std::cout << "Message encoded successfully into " << outputImagePath << "\n";
}

void encodeFile() {
    std::string inputImagePath, outputImagePath, inputFilePath, password;

    std::cout << "\n--- Encode File into PNG ---\n";

    // Get input image path
    std::cout << "Enter the path to the input PNG image: ";
    std::getline(std::cin, inputImagePath);

    // Get output image path
    std::cout << "Enter the desired path for the output PNG image: ";
    std::getline(std::cin, outputImagePath);

    // Get input file path
    std::cout << "Enter the path to the file to hide: ";
    std::getline(std::cin, inputFilePath);

    // Get password for encoding
    while (true) {
        std::cout << "Enter an 8-character password for encoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Encode the file into the image with the provided password
    if (encodeFileInPNG(inputImagePath, outputImagePath, inputFilePath, password)) {
        std::cout << "File encoded successfully into " << outputImagePath << "\n";
    } else {
        std::cerr << "Failed to encode the file into the image.\n";
    }
}

void decodeMessage() {
    std::string imagePath, password;
    std::cout << "\n--- Decode Message from PNG ---\n";

    // Get image path
    std::cout << "Enter the path to the PNG image: ";
    std::getline(std::cin, imagePath);

    // Get password for decoding
    while (true) {
        std::cout << "Enter the 8-character password for decoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Decode the message from the image with the provided password
    std::string decodedMessage = decodeMessageFromPNG(imagePath, password);
    if (!decodedMessage.empty()) {
        std::cout << "Decoded Message: " << decodedMessage << "\n";
    } else {
        std::cerr << "Failed to decode the message. Possible incorrect password or corrupted data.\n";
    }
}

void decodeFile() {
    std::string imagePath, outputFilePath, password;
    std::cout << "\n--- Decode File from PNG ---\n";

    // Get image path
    std::cout << "Enter the path to the PNG image: ";
    std::getline(std::cin, imagePath);

    // Get output file path
    std::cout << "Enter the desired path for the output file: ";
    std::getline(std::cin, outputFilePath);

    // Get password for decoding
    while (true) {
        std::cout << "Enter the 8-character password for decoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Decode the file from the image with the provided password
    if (decodeFileFromPNG(imagePath, outputFilePath, password)) {
        std::cout << "File decoded successfully to " << outputFilePath << "\n";
    } else {
        std::cerr << "Failed to decode the file. Possible incorrect password or corrupted data.\n";
    }
}
