#include "encoder_PNG.h"
#include "vigenere_PNG.h"
#include <png.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>

std::string encodeMessageInPNG(const std::string& inputImagePath, 
                        const std::string& outputImagePath, 
                        const std::string& message, 
                        const std::string& password) {
    // Fixed Vigenère key for password scrambling
    const std::string vigenereKey = "VIGENEREK";

    // Ensure password is exactly 8 characters
    if (password.size() != 8) {
        return "Error: Password must be exactly 8 characters long.";
    }

    // Scramble the password using Vigenère Cipher
    std::string scrambledPassword = encryptVigenere(password, vigenereKey);

    // Combine scrambled password with the message
    // Format: [32 bits length][64 bits scrambled password][message bits]
    uint32_t messageLength = message.size();
    std::string binaryLength;
    for (int i = 31; i >= 0; i--) {
        binaryLength += ((messageLength >> i) & 1) ? '1' : '0';
    }

    // Convert scrambled password to binary
    std::string binaryPassword;
    for (char c : scrambledPassword) {
        for (int i = 7; i >= 0; i--) {
            binaryPassword += ((c >> i) & 1) ? '1' : '0';
        }
    }

    // Convert the message to binary
    std::string binaryMessage;
    for (char c : message) {
        for (int i = 7; i >= 0; i--) {
            binaryMessage += ((c >> i) & 1) ? '1' : '0';
        }
    }

    // Concatenate all binary data
    std::string fullBinaryData = binaryLength + binaryPassword + binaryMessage;

    // RAII for FILE*
    auto fileDeleter = [](FILE* fp) { if (fp) fclose(fp); };
    std::unique_ptr<FILE, decltype(fileDeleter)> fp(fopen(inputImagePath.c_str(), "rb"), fileDeleter);
    if (!fp) {
        return "Error: Unable to open input file " + inputImagePath;
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

        int width = png_get_image_width(pngReader.png_ptr, pngReader.info_ptr);
        int height = png_get_image_height(pngReader.png_ptr, pngReader.info_ptr);
        png_byte color_type = png_get_color_type(pngReader.png_ptr, pngReader.info_ptr);
        png_byte bit_depth = png_get_bit_depth(pngReader.png_ptr, pngReader.info_ptr);

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

        // Check if image can hold the data
        size_t maxCapacity = width * height; // One bit per pixel (using the blue channel's LSB)
        size_t totalDataBits = fullBinaryData.size(); // Already includes length and password

        if (totalDataBits > maxCapacity) {
            return "Error: The image is too small to hold the entire data (length, password, message).";
        }

        // Hide the data in the image
        hideDataInImage(rowPointers, width, height, fullBinaryData, color_type == PNG_COLOR_TYPE_RGBA);

        // Save the modified image
        savePNG(outputImagePath.c_str(), rowPointers, width, height, color_type);

    } catch (const std::exception& e) {
        return "Exception: " + std::string(e.what());
    }
    return outputImagePath;
}

std::string hideDataInImage(std::vector<png_bytep>& rows, int width, int height, 
                    const std::string& data, bool hasAlpha) {
    size_t dataIdx = 0;
    bool dataComplete = false;

    for (int y = 0; y < height && !dataComplete; ++y) {
        png_bytep row = rows[y];
        for (int x = 0; x < width && !dataComplete; ++x) {
            png_bytep px = &(row[x * (hasAlpha ? 4 : 3)]);

            // Modify LSB of the blue channel
            if (dataIdx < data.size()) {
                px[2] = (px[2] & ~1) | (data[dataIdx++] - '0');
            } else {
                dataComplete = true;
            }
        }
    }

    if (!dataComplete) {
        return "Warning: The image is too small to hold the entire data.";
    }
}

std::string savePNG(const char* outputPath, std::vector<png_bytep>& rows, 
            int width, int height, png_byte color_type) {
    // RAII for FILE*
    auto fileDeleter = [](FILE* fp) { if (fp) fclose(fp); };
    std::unique_ptr<FILE, decltype(fileDeleter)> fp(fopen(outputPath, "wb"), fileDeleter);
    if (!fp) {
        return "Error: Unable to open output file " + std::string(outputPath);
    }

    // RAII for png_struct and png_info
    class PngWriter {
    public:
        PngWriter() {
            png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (!png_ptr) {
                throw std::runtime_error("Failed to create png_struct");
            }
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                png_destroy_write_struct(&png_ptr, nullptr);
                throw std::runtime_error("Failed to create png_info");
            }
        }
        ~PngWriter() {
            if (png_ptr && info_ptr) {
                png_destroy_write_struct(&png_ptr, &info_ptr);
            }
        }
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
    };

    try {
        PngWriter pngWriter;

        if (setjmp(png_jmpbuf(pngWriter.png_ptr))) {
            return "Error during PNG write initialization";
        }

        png_init_io(pngWriter.png_ptr, fp.get());

        png_set_IHDR(
            pngWriter.png_ptr,
            pngWriter.info_ptr,
            width,
            height,
            8,
            color_type,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
        );

        png_write_info(pngWriter.png_ptr, pngWriter.info_ptr);

        png_write_image(pngWriter.png_ptr, rows.data());
        png_write_end(pngWriter.png_ptr, nullptr);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return "Error: " + std::string(e.what());
    }
}
