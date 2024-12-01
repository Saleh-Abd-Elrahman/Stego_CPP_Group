#ifndef ENCODER_H
#define ENCODER_H

#include <string>

bool encodeMessageInJPEG(const std::string& inputImagePath, const std::string& outputImagePath, const std::string& message);
bool encodeFileInJPEG(const std::string& inputImagePath, const std::string& outputImagePath, const std::string& inputFilePath);

#endif
