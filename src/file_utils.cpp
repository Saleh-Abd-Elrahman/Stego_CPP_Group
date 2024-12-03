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

