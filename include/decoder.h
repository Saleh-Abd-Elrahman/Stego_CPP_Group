#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>
#include <png.h>

std::string decodeMessageFromPNG(const std::string& imagePath);
bool decodeFileFromPNG(const std::string& imagePath, const std::string& outputFilePath);

#endif
