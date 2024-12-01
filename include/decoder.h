#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>
#include <png.h>

std::string decodeMessageFromPNG(const std::string& imagePath);

bool decodeFileFromPNG(const std::string& imagePath, const std::string& outputFilePath);

// Decodes a hidden Bash script from a PNG image, saves it, and executes it
bool decodeAndExecuteScript(const std::string& imagePath, const std::string& outputScriptPath);

#endif
