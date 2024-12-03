#include "file_utils.h"
#include <cstddef> // for size_t
#include <string>
#include <fstream>
#include <stdexcept>

std::string getFileExtension(const std::string& filePath) {
    size_t dotPosition = filePath.find_last_of(".");
    if (dotPosition == std::string::npos) {
        return ""; // No extension found
    }
    return filePath.substr(dotPosition + 1);
}


std::string readFileContent(const std::string& filePath) {
    std::ifstream inFile(filePath); 
    if (!inFile.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filePath);
    }

    // Seek to the end to determine the file size
    inFile.seekg(0, std::ios::end);
    std::streamsize fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    // Read the file content into a string
    std::string fileContent(fileSize, '\0'); // Preallocate memory for the content
    if (!inFile.read(&fileContent[0], fileSize)) {
        throw std::runtime_error("Error: Failed to read file content.");
    }

    return fileContent;
}