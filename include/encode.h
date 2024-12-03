#ifndef ENCODE_H
#define ENCODE_H

#include <string>

// Function declarations
std::string encodeMessage(const std::string& targetFilePath, const std::string& message, const std::string& password);
std::string encodeText(const std::string& targetFilePath, const std::string& textFilePath, const std::string& password);

#endif // ENCODE_H