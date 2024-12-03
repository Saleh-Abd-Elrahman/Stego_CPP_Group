#include "file_utils.h"
#include <cstddef> // for size_t
#include <string>

std::string getFileExtension(const std::string& filePath) {
    size_t dotPosition = filePath.find_last_of(".");
    if (dotPosition == std::string::npos) {
        return ""; // No extension found
    }
    return filePath.substr(dotPosition + 1);
}

std::string getDirectoryPath(const std::string& filePath) {
    size_t slashPosition = filePath.find_last_of("/\\");
    if (slashPosition == std::string::npos) {
        return ""; // No directory part found
    }
    return filePath.substr(0, slashPosition);
}
