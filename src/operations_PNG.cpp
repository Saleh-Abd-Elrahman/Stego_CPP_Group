#include "operations_PNG.h"
#include "encoder_PNG.h"
#include "decoder_PNG.h"
#include "Steganography.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstring>

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

    // Validate that the input file exists
    std::ifstream infile(inputFilePath, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: File " << inputFilePath << " does not exist or cannot be opened.\n";
        return;
    }
    infile.close();

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

void encodePNGFile() {
    std::string inputImagePath, outputImagePath, inputPNGPath, password;

    std::cout << "\n--- Encode PNG File into PNG ---\n";

    // Get carrier image path
    std::cout << "Enter the path to the carrier PNG image: ";
    std::getline(std::cin, inputImagePath);

    // Get output image path
    std::cout << "Enter the desired path for the output PNG image: ";
    std::getline(std::cin, outputImagePath);

    // Get input PNG file path to hide
    std::cout << "Enter the path to the PNG file to hide: ";
    std::getline(std::cin, inputPNGPath);

    // Validate that the input PNG file exists and is a valid PNG
    if (!isValidPNG(inputPNGPath)) {
        std::cerr << "Error: The file " << inputPNGPath << " is not a valid PNG image or cannot be opened.\n";
        return;
    }

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

    // Encode the PNG file into the carrier image with the provided password
    if (encodeFileInPNG(inputImagePath, outputImagePath, inputPNGPath, password)) {
        std::cout << "PNG file encoded successfully into " << outputImagePath << "\n";
    } else {
        std::cerr << "Failed to encode the PNG file into the image.\n";
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

void decodePNGFile() {
    std::string imagePath, outputPNGPath, password;
    std::cout << "\n--- Decode PNG File from PNG ---\n";

    // Get image path
    std::cout << "Enter the path to the PNG image: ";
    std::getline(std::cin, imagePath);

    // Get output PNG file path
    std::cout << "Enter the desired path for the output PNG file: ";
    std::getline(std::cin, outputPNGPath);

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

    // Decode the PNG file from the image with the provided password
    if (decodeFileFromPNG(imagePath, outputPNGPath, password)) {
        std::cout << "PNG file decoded successfully to " << outputPNGPath << "\n";
    } else {
        std::cerr << "Failed to decode the PNG file. Possible incorrect password or corrupted data.\n";
    }
}

bool isValidPNG(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return false;
    }

    // Read the first 8 bytes of the file to check the PNG signature
    unsigned char signature[8];
    file.read(reinterpret_cast<char*>(signature), 8);
    if (file.gcount() != 8) {
        return false;
    }

    const unsigned char pngSignature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    return std::memcmp(signature, pngSignature, 8) == 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void encodeDirectMessagWAV(){
    // Encode Message
    std::cout << "\n--- Encode a Message into WAV ---\n";

    // Get input WAV path
    std::cout << "Enter the path to the input WAV file: ";
    std::getline(std::cin, inputWav);

    // Get output WAV path
    std::cout << "Enter the desired path for the output WAV file: ";
    std::getline(std::cin, outputWav);

    // Get message to encode
    std::cout << "Enter the message to hide: ";
    std::getline(std::cin, message);

    // Get password
    while (true) {
        std::cout << "Enter an 8-character password for encoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Encode the message
    if (Steganography::encodeMessageInWAV(inputWav, outputWav, message, password)) {
        std::cout << "Message encoded successfully into " << outputWav << "\n";
    } else {
        std::cerr << "Failed to encode the message.\n";
    }

}

void decodeDirectMessageWAV(){
    // Decode Message
    std::cout << "\n--- Decode a Message from WAV ---\n";

    // Get input WAV path
    std::cout << "Enter the path to the WAV file: ";
    std::getline(std::cin, inputWav);

    // Get password
    while (true) {
        std::cout << "Enter the 8-character password for decoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Decode the message
    std::string decodedMessage = Steganography::decodeMessageInWAV(inputWav, password);
    if (!decodedMessage.empty()) {
        std::cout << "Decoded Message: " << decodedMessage << "\n";
    } else {
        std::cerr << "Failed to decode the message.\n";
    }

}

void EncodeFileWAV(){
    // Encode File
    std::cout << "\n--- Encode a File into WAV ---\n";

    // Get input WAV path
    std::cout << "Enter the path to the input WAV file: ";
    std::getline(std::cin, inputWav);

    // Get output WAV path
    std::cout << "Enter the desired path for the output WAV file: ";
    std::getline(std::cin, outputWav);

    // Get input file path to encode
    std::cout << "Enter the path to the file to hide (text, PNG, Bash script): ";
    std::getline(std::cin, inputFilePath);

    // Get password
    while (true) {
        std::cout << "Enter an 8-character password for encoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Encode the file
    if (Steganography::encodeFileInWAV(inputWav, outputWav, inputFilePath, password)) {
        std::cout << "File encoded successfully into " << outputWav << "\n";
    } else {
        std::cerr << "Failed to encode the file.\n";
    }
}

void DecodeFileWAV(){
    // Decode File
    std::cout << "\n--- Decode a File from WAV ---\n";

    // Get input WAV path
    std::cout << "Enter the path to the WAV file: ";
    std::getline(std::cin, inputWav);

    // Get output file path
    std::cout << "Enter the desired path for the output file: ";
    std::getline(std::cin, outputFilePath);

    // Get password
    while (true) {
        std::cout << "Enter the 8-character password for decoding: ";
        std::getline(std::cin, password);
        if (password.size() == 8) {
            break;
        } else {
            std::cerr << "Error: Password must be exactly 8 characters long.\n";
        }
    }

    // Decode the file
    if (Steganography::decodeFileInWAV(inputWav, outputFilePath, password)) {
        std::cout << "File decoded successfully to " << outputFilePath << "\n";
    } else {
        std::cerr << "Failed to decode the file.\n";
    }
}



// Function to run the main menu loop
void runMenu() {
    int choice = 0;

    while (true) {
        std::cout << "\n=== Steganography Application ===\n";
        std::cout << "1. Encode a Direct Message into PNG\n";
        std::cout << "2. Encode a File into PNG\n";
        std::cout << "3. Encode a PNG File into PNG\n";
        std::cout << "4. Decode a Message from PNG\n";
        std::cout << "5. Decode a File from PNG\n";
        std::cout << "6. Decode a PNG File from PNG\n";
        std::cout << "7. Encode a Direct Message from WAV\n";
        std::cout << "8. Decode a Direct Message from WAV\n";
        std::cout << "9. Encode a Direct Message from WAV\n";
        std::cout << "10. Decode a Direct Message from WAV\n";
        std::cout << "11. Exit\n";
        std::cout << "Enter your choice (1-7): ";
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
                encodePNGFile();
                break;
            case 4:
                decodeMessage();
                break;
            case 5:
                decodeFile();
                break;
            case 6:
                decodePNGFile();
                break;
            case 7:
                encodeDirectMessagWAV();
                break;
            case 8:
                decodeDirectMessageWAV();
                return;
            case 9:
                EncodeFileWAV();
                break;
            case 10:
                DecodeFileWAV();
                break;
            case 11:
                std::cout << "Exiting the application.\n";
                return;
            default:
                std::cerr << "Invalid choice. Please select a number between 1 and 7.\n";
        }
    }
}