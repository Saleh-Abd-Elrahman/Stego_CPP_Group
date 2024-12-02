#ifndef ENCODER_H
#define ENCODER_H

#include <png.h> 
#include <string>
#include <vector>

// Encodes a message into a PNG image with password protection
void encodeMessageInPNG(const std::string& inputImagePath, 
                        const std::string& outputImagePath, 
                        const std::string& message, 
                        const std::string& password);

// Encodes a file into a PNG image with password protection
bool encodeFileInPNG(const std::string& inputImagePath, 
                     const std::string& outputImagePath, 
                     const std::string& inputFilePath, 
                     const std::string& password);

// Helper functions
void hideDataInImage(std::vector<png_bytep>& rows, int width, int height, 
                    const std::string& data, bool hasAlpha);
void savePNG(const char* outputImagePath, std::vector<png_bytep>& rows, 
            int width, int height, png_byte colorType);

#endif // ENCODER_H
