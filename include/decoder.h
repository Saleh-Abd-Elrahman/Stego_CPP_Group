#ifndef DECODER_H
#define DECODER_H

#include <string>

std::string decodeMessageFromJPEG(const std::string& imagePath);
bool decodeFileFromJPEG(const std::string& imagePath, const std::string& outputFilePath);

#endif
