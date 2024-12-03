#include "decoder_PNG.h"
#include "vigenere_PNG.h"
#include <png.h>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <filesystem> // C++17 for setting permissions
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

// Fixed Vigenère key for password unscrambling
const std::string vigenereKey = "VIGENEREK";

// Helper function for countdown
void countdownErase() {
    std::cout << "Break In attempt detected, data deletion in..." << std::endl;
    for (int i = 5; i >= 0; --i) {
        std::cout << i << " ";
        std::flush(std::cout);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << std::endl;
}

// Helper function to erase hidden data by setting LSBs to 0
// Change return type to bool
bool eraseHiddenDataInPNG(const std::string& imagePath) {
    // RAII for FILE*
    auto fileDeleter = [](FILE* fp) { if (fp) fclose(fp); };
    std::unique_ptr<FILE, decltype(fileDeleter)> fp(fopen(imagePath.c_str(), "rb"), fileDeleter);
    if (!fp) {
        std::cerr << "Error: Unable to open file " << imagePath << std::endl;
        return false;
    }

    // RAII for png_struct and png_info
    class PngReaderWriter {
    public:
        PngReaderWriter() {
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (!png_ptr) {
                throw std::runtime_error("Failed to create png_struct");
            }
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                png_destroy_read_struct(&png_ptr, nullptr, nullptr);
                throw std::runtime_error("Failed to create png_info");
            }
            png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (!png_write_ptr) {
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                throw std::runtime_error("Failed to create png_write_struct");
            }
            write_info_ptr = png_create_info_struct(png_write_ptr);
            if (!write_info_ptr) {
                png_destroy_write_struct(&png_write_ptr, nullptr);
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                throw std::runtime_error("Failed to create png_write_info");
            }
        }
        ~PngReaderWriter() {
            if (png_ptr && info_ptr) {
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            }
            if (png_write_ptr && write_info_ptr) {
                png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
            }
        }
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        png_structp png_write_ptr = nullptr;
        png_infop write_info_ptr = nullptr;
    };

    try {
        PngReaderWriter pngRW;

        if (setjmp(png_jmpbuf(pngRW.png_ptr))) {
            std::cerr << "Error during PNG read initialization" << std::endl;
            return false;
        }

        png_init_io(pngRW.png_ptr, fp.get());
        png_read_info(pngRW.png_ptr, pngRW.info_ptr);

        int width = png_get_image_width(pngRW.png_ptr, pngRW.info_ptr);
        int height = png_get_image_height(pngRW.png_ptr, pngRW.info_ptr);
        png_byte color_type = png_get_color_type(pngRW.png_ptr, pngRW.info_ptr);
        png_byte bit_depth = png_get_bit_depth(pngRW.png_ptr, pngRW.info_ptr);

        if (bit_depth != 8 || 
           (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA)) {
            std::cerr << "Unsupported PNG format. Only 8-bit RGB or RGBA is supported." << std::endl;
            return false;
        }

        png_read_update_info(pngRW.png_ptr, pngRW.info_ptr);

        // Allocate memory for rows using std::vector
        std::vector<std::vector<png_byte>> imageData(height);
        std::vector<png_bytep> rowPointers(height);

        for (int y = 0; y < height; y++) {
            imageData[y].resize(png_get_rowbytes(pngRW.png_ptr, pngRW.info_ptr));
            rowPointers[y] = imageData[y].data();
        }

        png_read_image(pngRW.png_ptr, rowPointers.data());

        // Erase the LSBs by setting them to 0
        for (int y = 0; y < height; ++y) {
            png_bytep row = rowPointers[y];
            for (int x = 0; x < width; ++x) {
                png_bytep px = &(row[x * (color_type == PNG_COLOR_TYPE_RGBA ? 4 : 3)]);
                px[2] &= ~1; // Set LSB of blue channel to 0
            }
        }

        // Write the modified image back
        // Reopen the file for writing
        fp.reset(fopen(imagePath.c_str(), "wb"));
        if (!fp) {
            std::cerr << "Error: Unable to open file for writing " << imagePath << std::endl;
            return false;
        }

        png_init_io(pngRW.png_write_ptr, fp.get());

        png_set_IHDR(
            pngRW.png_write_ptr,
            pngRW.write_info_ptr,
            width,
            height,
            8,
            color_type,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
        );

        png_write_info(pngRW.png_write_ptr, pngRW.write_info_ptr);
        png_write_image(pngRW.png_write_ptr, rowPointers.data());
        png_write_end(pngRW.png_write_ptr, nullptr);

        return true; // Indicate success

    } catch (const std::exception& e) {
        std::cerr << "Exception during data erasure: " << e.what() << std::endl;
        return false;
    }
}

// Existing decodeMessageFromPNG implementation with password verification

std::string decodeMessageFromPNG(const std::string& imagePath, const std::string& password) {
    // RAII for FILE*
    auto fileDeleter = [](FILE* fp) { if (fp) fclose(fp); };
    std::unique_ptr<FILE, decltype(fileDeleter)> fp(fopen(imagePath.c_str(), "rb"), fileDeleter);
    if (!fp) {
        return "Error: Unable to open file " + imagePath;
    }

    // RAII for png_struct and png_info
    class PngReader {
    public:
        PngReader() {
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (!png_ptr) {
                throw std::runtime_error("Failed to create png_struct");
            }
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                png_destroy_read_struct(&png_ptr, nullptr, nullptr);
                throw std::runtime_error("Failed to create png_info");
            }
        }
        ~PngReader() {
            if (png_ptr && info_ptr) {
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            }
        }
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
    };

    try {
        PngReader pngReader;

        if (setjmp(png_jmpbuf(pngReader.png_ptr))) {
            return "Error during PNG read initialization";
        }

        png_init_io(pngReader.png_ptr, fp.get());
        png_read_info(pngReader.png_ptr, pngReader.info_ptr);

        int width      = png_get_image_width(pngReader.png_ptr, pngReader.info_ptr);
        int height     = png_get_image_height(pngReader.png_ptr, pngReader.info_ptr);
        png_byte color_type = png_get_color_type(pngReader.png_ptr, pngReader.info_ptr);
        png_byte bit_depth  = png_get_bit_depth(pngReader.png_ptr, pngReader.info_ptr);

        // Format check
        if (bit_depth != 8 || 
           (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA)) {
            return "Unsupported PNG format. Only 8-bit RGB or RGBA is supported.";
        }

        png_read_update_info(pngReader.png_ptr, pngReader.info_ptr);

        // Allocate memory for rows using std::vector
        std::vector<std::vector<png_byte>> imageData(height);
        std::vector<png_bytep> rowPointers(height);

        for (int y = 0; y < height; y++) {
            imageData[y].resize(png_get_rowbytes(pngReader.png_ptr, pngReader.info_ptr));
            rowPointers[y] = imageData[y].data();
        }

        png_read_image(pngReader.png_ptr, rowPointers.data());

        // Read bits from image
        std::string binaryData;
        bool hasAlpha = (color_type == PNG_COLOR_TYPE_RGBA);

        for (int y = 0; y < height; ++y) {
            png_bytep row = rowPointers[y];
            for (int x = 0; x < width; ++x) {
                png_bytep px = &(row[x * (hasAlpha ? 4 : 3)]);
                binaryData += (px[2] & 1) ? '1' : '0';
            }
        }

        // Read the first 32 bits to get the message length
        if (binaryData.size() < 32 + 64) { // 32 bits length + 64 bits password
            return "Error: Not enough data to read message length and password.";
        }

        uint32_t messageLength = 0;
        for (int i = 0; i < 32; i++) {
            messageLength = (messageLength << 1) | (binaryData[i] - '0');
        }

        // Read the next 64 bits for scrambled password
        std::string binaryPassword = binaryData.substr(32, 64);
        std::string scrambledPassword;
        for (size_t i = 0; i < binaryPassword.size(); i += 8) {
            std::string byteString = binaryPassword.substr(i, 8);
            char byte = static_cast<char>(std::stoi(byteString, nullptr, 2));
            scrambledPassword += byte;
        }

        // Unscramble the password
        std::string unscrambledPassword = decryptVigenere(scrambledPassword, vigenereKey);

        // Compare with user-provided password
        if (unscrambledPassword != password) {
            countdownErase();
            if (eraseHiddenDataInPNG(imagePath)) {
                return "Hidden data has been permanently erased.";
            } else {
                return "Failed to erase hidden data.";
            }
        }

        // Now read the message bits
        size_t totalMessageBits = messageLength * 8;
        if (binaryData.size() < 32 + 64 + totalMessageBits) {
            return "Error: Not enough data to read the entire message.";
        }

        std::string binaryMessage = binaryData.substr(32 + 64, totalMessageBits);

        std::string decodedMessage;
        decodedMessage.reserve(messageLength);
        for (size_t i = 0; i < binaryMessage.size(); i += 8) {
            std::string byteString = binaryMessage.substr(i, 8);
            char byte = static_cast<char>(std::stoi(byteString, nullptr, 2));
            decodedMessage += byte;
        }

        return decodedMessage;

    } catch (const std::exception& e) {
        return "Exception: " + std::string(e.what());
    }
}


std::string decodeFileFromPNG(const std::string& imagePath, 
                       const std::string& outputFilePath, 
                       const std::string& password) {
    std::string decodedData = decodeMessageFromPNG(imagePath, password);
    if (decodedData.empty()) {
        return "Error: Decoded data is empty.";
    }

    // Check for error messages
    if (decodedData.find("Error:") == 0 || 
        decodedData.find("Exception:") == 0 || 
        decodedData.find("Hidden data has been permanently erased.") == 0 || 
        decodedData.find("Failed to erase hidden data.") == 0) {
        // Propagate the error message
        return decodedData;
    }

    // Write the decoded data to the output file in binary mode
    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile) {
        return "Error: Unable to open output file " + outputFilePath;
    }
    outFile.write(decodedData.data(), decodedData.size());
    outFile.close();
    return "";
}

bool decodeAndExecuteScript(const std::string& imagePath, 
                            const std::string& outputScriptPath, 
                            const std::string& password) {

    std::string decodeResult = decodeFileFromPNG(imagePath, outputScriptPath, password);
    if (!decodeResult.empty()) { // If there's an error message
        std::cerr << decodeResult << std::endl;
        return false;
    }

    // Set executable permissions using std::filesystem (C++17)
    try {
        fs::permissions(outputScriptPath,
                        fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec,
                        fs::perm_options::add);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error while setting permissions: " << e.what() << std::endl;
        return false;
    }

    // Execute the script using system()
    std::string command = "bash \"" + outputScriptPath + "\"";
    int ret = system(command.c_str());
    if (ret != 0) {
        std::cerr << "Error: Script execution failed with return code " << ret << std::endl;
        return false;
    }

    return true;
}
