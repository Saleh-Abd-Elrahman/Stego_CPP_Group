#ifndef DECODE_H
#define DECODE_H

#include <string>

/**
 * @brief Decodes a message from a file using a password.
 * 
 * This function reads the content of the file specified by `filePath` and decodes it using
 * the provided `password`. It returns the decoded message as a string.
 * 
 * @param filePath The path to the file to decode.
 * @param password The password used to decode the message.
 * @return A string containing the decoded message.
 */
std::string decodeMessage(const std::string& filePath, const std::string& password);

/**
 * @brief Decodes a file using a password.
 * 
 * This function reads the file specified by `filePath` and decodes it using the given password.
 * It returns the decoded content of the file as a string.
 * 
 * @param filePath The path to the file to decode.
 * @param password The password used to decode the file.
 * @return A string containing the decoded file content.
 */
std::string decodeFile(const std::string& filePath, const std::string& password);

/**
 * @brief Decodes a Bash script using a password.
 * 
 * This function processes the Bash script file at the specified `filePath` and decodes it using
 * the provided password. The decoded content is returned as a string.
 * 
 * @param filePath The path to the Bash script file to decode.
 * @param password The password used to decode the Bash script.
 * @return A string containing the decoded Bash script.
 */
std::string decodeBashScript(const std::string& filePath, const std::string& password);

#endif // DECODE_H
