#include "encode.h"
#include "file_utils.h"
#include "encoder_PNG.h"
#include "WAV.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <filesystem>
// Encode a message
std::string encodeMessage(const std::string& targetFilePath, const std::string& message, const std::string& password) {

    std::string fileExtension = getFileExtension(targetFilePath);
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
    
    namespace fs = std::filesystem;

    if (!fs::exists(targetFilePath)) {
        return "Error: Input file does not exist.";
    }

    fs::path inputPath(targetFilePath);
    fs::path outputImagePath = inputPath.parent_path() / (inputPath.stem().string() + "_encoded" + inputPath.extension().string());

    if (fileExtension == "png") {
        encodeMessageInPNG(targetFilePath, outputImagePath, message, password);
        return "Encoded text file into PNG: " + outputImagePath.string();
    } else if (fileExtension == "wav") {
        Steganography::encodeMessageInWAV(targetFilePath, outputImagePath, message, password);
        return "Encoded text file into WAV: " + outputImagePath.string();
    } else {
        throw std::runtime_error("Unsupported file format: " + fileExtension);
    }
}

// Encode the content of a text file
std::string encodeText(const std::string& targetFilePath, const std::string& textFilePath, const std::string& password) {
    std::string fileExtension = getFileExtension(targetFilePath);
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    namespace fs = std::filesystem;

    if (!fs::exists(targetFilePath)) {
        return "Error: Input file does not exist.";
    }
    fs::path inputPath(targetFilePath);
    fs::path outputPath = inputPath.parent_path() / (inputPath.stem().string() + "_encoded" + inputPath.extension().string());
    
    if (fileExtension == "png") {
        // Read the content of the input file in binary mode
        std::ifstream inFile(textFilePath, std::ios::binary);
        if (!inFile) {
            return "Error: Unable to open file " + textFilePath;
        }
        std::string fileContent((std::istreambuf_iterator<char>(inFile)),  std::istreambuf_iterator<char>());
        inFile.close();
        encodeMessageInPNG(targetFilePath, outputPath, fileContent, password);

        return "Encoded text file into PNG: " + outputPath.string();
    } else if (fileExtension == "wav") {
        Steganography::encodeFileInWAV(targetFilePath, outputPath, textFilePath, password);
        return "Encoded text file into WAV: " + outputPath.string();
    } else {
        throw std::runtime_error("Unsupported file format: " + fileExtension);
    }
}

