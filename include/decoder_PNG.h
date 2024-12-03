#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>
#include <png.h>

/**
 * @file decoder.h
 * @brief Provides functions to decode hidden messages and files from PNG images with password verification.
 */

/**
 * @brief Decodes a hidden text message from a PNG image after verifying the password.
 *
 * This function extracts a hidden text message from the least significant bits of the blue channel
 * in each pixel of the PNG image. It first verifies the provided password by comparing it with the
 * scrambled password embedded within the image. If the password matches, it retrieves and returns
 * the hidden message.
 *
 * @param imagePath Path to the PNG image containing the hidden message.
 * @param password The 8-character password used during encoding to scramble the message.
 * @return The decoded text message if successful; an empty string otherwise.
 */
std::string decodeMessageFromPNG(const std::string& imagePath, const std::string& password);

/**
 * @brief Decodes a hidden binary file from a PNG image after verifying the password.
 *
 * This function extracts a hidden binary file from the least significant bits of the blue channel
 * in each pixel of the PNG image. It first verifies the provided password by comparing it with the
 * scrambled password embedded within the image. If the password matches, it retrieves and saves
 * the hidden file to the specified output path.
 *
 * @param imagePath Path to the PNG image containing the hidden file.
 * @param outputFilePath Path where the decoded file will be saved.
 * @param password The 8-character password used during encoding to scramble the file data.
 * @return `true` if decoding and saving are successful; `false` otherwise.
 */
bool decodeFileFromPNG(const std::string& imagePath, 
                       const std::string& outputFilePath, 
                       const std::string& password);

/**
 * @brief Decodes a hidden Bash script from a PNG image, saves it, and executes it after verifying the password.
 *
 * This function extracts a hidden Bash script from the least significant bits of the blue channel
 * in each pixel of the PNG image. It verifies the provided password by comparing it with the scrambled
 * password embedded within the image. Upon successful verification, it saves the script to the specified
 * output path, sets executable permissions, and executes the script.
 *
 * @param imagePath Path to the PNG image containing the hidden Bash script.
 * @param outputScriptPath Path where the decoded Bash script will be saved.
 * @param password The 8-character password used during encoding to scramble the script data.
 * @return `true` if decoding, saving, and execution are successful; `false` otherwise.
 */
bool decodeAndExecuteScript(const std::string& imagePath, 
                            const std::string& outputScriptPath, 
                            const std::string& password);

/**
 * @brief Erases hidden data from a PNG image by resetting the least significant bits.
 *
 * This helper function removes any hidden data embedded within the image by setting the least
 * significant bits of the blue channel in each pixel to zero. This effectively deletes the
 * hidden message or file from the PNG image.
 *
 * @param imagePath Path to the PNG image from which the hidden data will be erased.
 * @return `true` if the hidden data is successfully erased; `false` otherwise.
 */
bool eraseHiddenDataInPNG(const std::string& imagePath);

#endif // DECODER_H
