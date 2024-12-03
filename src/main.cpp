#include "Steganography.h"
#include <iostream>
#include <string>

int main() {
    int choice = 0;
    std::string inputWav, outputWav, message, password, inputFilePath, outputFilePath;

    while (true) {
        std::cout << "\n=== Steganography Application ===\n";
        std::cout << "1. Encode a Message into WAV\n";
        std::cout << "2. Decode a Message from WAV\n";
        std::cout << "3. Encode a File into WAV\n";
        std::cout << "4. Decode a File from WAV\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;

        // Clear input buffer to handle any leftover input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
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
            if (Steganography::encodeMessage(inputWav, outputWav, message, password)) {
                std::cout << "Message encoded successfully into " << outputWav << "\n";
            } else {
                std::cerr << "Failed to encode the message.\n";
            }

        } else if (choice == 2) {
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
            std::string decodedMessage = Steganography::decodeMessage(inputWav, password);
            if (!decodedMessage.empty()) {
                std::cout << "Decoded Message: " << decodedMessage << "\n";
            } else {
                std::cerr << "Failed to decode the message.\n";
            }

        } else if (choice == 3) {
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
            if (Steganography::encodeFile(inputWav, outputWav, inputFilePath, password)) {
                std::cout << "File encoded successfully into " << outputWav << "\n";
            } else {
                std::cerr << "Failed to encode the file.\n";
            }

        } else if (choice == 4) {
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
            if (Steganography::decodeFile(inputWav, outputFilePath, password)) {
                std::cout << "File decoded successfully to " << outputFilePath << "\n";
            } else {
                std::cerr << "Failed to decode the file.\n";
            }

        } else if (choice == 5) {
            // Exit
            std::cout << "Exiting the application. Goodbye!\n";
            break;
        } else {
            std::cerr << "Invalid choice. Please select a number between 1 and 5.\n";
        }
    }

    return 0;
}
