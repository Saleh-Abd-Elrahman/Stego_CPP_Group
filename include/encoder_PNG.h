#ifndef ENCODER_H
#define ENCODER_H

#include <png.h>
#include <string>
#include <vector>

/**
 * @file encoder.h
 * @brief Provides functions to encode messages and files into PNG images with password protection.
 */

/**
 * @brief Encodes a text message into a PNG image using steganography.
 *
 * This function hides a given text message within the least significant bits of the blue channel
 * of each pixel in the carrier PNG image. The message is protected with an 8-character password
 * using the Vigenère Cipher.
 *
 * @param inputImagePath Path to the input (carrier) PNG image.
 * @param outputImagePath Path where the output PNG image with the hidden message will be saved.
 * @param message The text message to hide within the PNG image.
 * @param password An 8-character password used to scramble the message for protection.
 */
std::string encodeMessageInPNG(const std::string& inputImagePath, 
                        const std::string& outputImagePath, 
                        const std::string& message, 
                        const std::string& password);

/**
 * @brief Hides binary data within the image data by modifying the least significant bits.
 *
 * This helper function embeds a binary string into the image's pixel data by altering the
 * least significant bits (LSBs) of the blue channel. It iterates through each pixel and
 * modifies the LSB based on the provided data.
 *
 * @param rows A vector of row pointers representing the image's pixel data.
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param data A binary string representing the data to hide within the image.
 * @param hasAlpha Indicates whether the image has an alpha channel (RGBA) or not (RGB).
 */
std::string hideDataInImage(std::vector<png_bytep>& rows, int width, int height, 
                    const std::string& data, bool hasAlpha);

/**
 * @brief Saves the modified image data back to a PNG file.
 *
 * This helper function writes the pixel data back to a new PNG file using the libpng library.
 *
 * @param outputImagePath Path where the output PNG image will be saved.
 * @param rows A vector of row pointers representing the image's pixel data.
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param colorType The color type of the PNG image (e.g., PNG_COLOR_TYPE_RGB or PNG_COLOR_TYPE_RGBA).
 */
std::string savePNG(const char* outputImagePath, std::vector<png_bytep>& rows, 
            int width, int height, png_byte colorType);

#endif // ENCODER_H
