#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

/**
 * @brief Retrieves the file extension from a file path.
 * 
 * This function extracts and returns the file extension from the given `filePath`.
 * The extension is returned as a string, including the dot (e.g., ".txt", ".png").
 * 
 * @param filePath The path to the file from which to extract the extension.
 * @return A string containing the file extension.
 */
std::string getFileExtension(const std::string& filePath);

/**
 * @brief Reads the content of a file.
 * 
 * This function reads the content of the file specified by `filePath` and returns it
 * as a string. The function assumes that the file is accessible and its content is
 * text-based.
 * 
 * @param filePath The path to the file to read.
 * @return A string containing the content of the file.
 */
std::string readFileContent(const std::string& filePath);

#endif // FILE_UTILS_H
