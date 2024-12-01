#ifndef ENCODE_H
#define ENCODE_H

#include <string>

// Function declarations
std::string encodeMessage(const std::string& targetFilePath, const std::string& message);
std::string encodeText(const std::string& targetFilePath, const std::string& textFilePath);

#endif // ENCODE_H