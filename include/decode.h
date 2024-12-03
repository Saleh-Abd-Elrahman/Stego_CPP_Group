#ifndef DECODE_H
#define DECODE_H

#include <string>

// Function declaration
std::string decodeMessage(const std::string& filePath, const std::string& password) ;
std::string decodeFile(const std::string& filePath, const std::string& password);
std::string decodeBashScript(const std::string& filePath, const std::string& password);
#endif // DECODE_H