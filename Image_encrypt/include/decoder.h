#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>
#include <png.h>

// Decodes a hidden message from a PNG image with password verification
std::string decodeMessageFromPNG(const std::string& imagePath, const std::string& password);

// Decodes a hidden file from a PNG image with password verification
bool decodeFileFromPNG(const std::string& imagePath, 
                       const std::string& outputFilePath, 
                       const std::string& password);

// Decodes a hidden Bash script from a PNG image, saves it, and executes it with password verification
bool decodeAndExecuteScript(const std::string& imagePath, 
                            const std::string& outputScriptPath, 
                            const std::string& password);

// Helper functions
bool eraseHiddenDataInPNG(const std::string& imagePath);

#endif // DECODER_H
