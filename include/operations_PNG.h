#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>

/**
 * @file operations.h
 * @brief Declares functions to handle user interactions for encoding and decoding operations.
 */

/**
 * @brief Encodes a direct text message into a PNG image.
 *
 * This function interacts with the user to obtain the necessary inputs, such as the carrier image path,
 * output image path, message to hide, and password. It then calls the encoder functions to perform the
 * steganography operation.
 */
void encodeDirectMessage();

/**
 * @brief Encodes a binary file into a PNG image.
 *
 * This function interacts with the user to obtain the necessary inputs, such as the carrier image path,
 * output image path, file to hide, and password. It validates the file and then calls the encoder
 * functions to perform the steganography operation.
 */
void encodeFile();

/**
 * @brief Encodes a PNG file into another PNG image.
 *
 * This function allows users to hide an entire PNG image within another PNG image. It interacts with
 * the user to obtain the carrier image path, output image path, PNG file to hide, and password. It
 * validates the PNG file before performing the encoding.
 */
void encodePNGFile();

/**
 * @brief Decodes a hidden text message from a PNG image after password verification.
 *
 * This function interacts with the user to obtain the necessary inputs, such as the image path and
 * password. It then calls the decoder functions to extract and display the hidden message.
 */
void decodeMessage();

/**
 * @brief Decodes a hidden binary file from a PNG image after password verification.
 *
 * This function interacts with the user to obtain the necessary inputs, such as the image path,
 * output file path, and password. It then calls the decoder functions to extract and save the hidden
 * file.
 */
void decodeFile();

/**
 * @brief Decodes a hidden PNG file from another PNG image after password verification.
 *
 * This function allows users to extract a hidden PNG image from a carrier PNG image. It interacts
 * with the user to obtain the image path, output PNG file path, and password, then performs the
 * decoding operation.
 */
void decodePNGFile();

/**
 * @brief Runs the main menu loop for the steganography application.
 *
 * This function displays a menu to the user with options to encode or decode various types of
 * data (messages, files, PNG images) into or from PNG images. It handles user input and invokes
 * the corresponding functions based on the user's choice.
 */
void runMenu();

/**
 * @brief Validates whether a given file is a valid PNG image by checking its signature.
 *
 * This helper function reads the first eight bytes of the file to verify the PNG signature.
 *
 * @param filePath The path to the file to validate.
 * @return `true` if the file is a valid PNG image; `false` otherwise.
 */
bool isValidPNG(const std::string& filePath);

#endif // OPERATIONS_H
