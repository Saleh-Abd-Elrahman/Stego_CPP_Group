#ifndef ENCODE_H
#define ENCODE_H

#include <string>

/**
 * @brief Encodes a message into a file using a password.
 * 
 * This function encodes the provided message into a file specified by `targetFilePath`.
 * The encoding is performed using the provided password. The encoded content is
 * saved to the target file, and a string containing the encoded message is returned.
 * 
 * @param targetFilePath The path to the target file where the encoded message will be saved.
 * @param message The message to encode.
 * @param password The password used for encoding the message.
 * @return A string containing the encoded message.
 */
std::string encodeMessage(const std::string& targetFilePath, const std::string& message, const std::string& password);

/**
 * @brief Encodes text from a file into another file using a password.
 * 
 * This function reads text from a file specified by `textFilePath`, encodes it, and
 * saves the encoded text to a target file at `targetFilePath`. The encoding is performed
 * using the provided password.
 * 
 * @param targetFilePath The path to the target file where the encoded text will be saved.
 * @param textFilePath The path to the text file containing the content to encode.
 * @param password The password used for encoding the text.
 * @return A string containing the encoded text.
 */
std::string encodeText(const std::string& targetFilePath, const std::string& textFilePath, const std::string& password);

#endif // ENCODE_H
