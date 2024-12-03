#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

// Function to get the file extension from a file path
std::string getFileExtension(const std::string& filePath);

std::string getDirectoryPath(const std::string& filePath);

#endif // FILE_UTILS_H
