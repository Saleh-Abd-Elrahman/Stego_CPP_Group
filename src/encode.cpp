#include "encode.h"
#include "file_utils.h"
#include "encoder_PNG.h"
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
        std::cerr << "Error: Input file does not exist." << std::endl;
        return;
    }

    fs::path inputPath(targetFilePath);
    fs::path outputImagePath = inputPath.parent_path() / (inputPath.stem().string() + "_encoded" + inputPath.extension().string());

    if (fileExtension == "png") {
        encodeMessageInPNG(targetFilePath, outputImagePath, message, password);
        return "Encoded text file into PNG: " + targetFilePath;
    } else if (fileExtension == "wav") {
        // Add logic for encoding text content into a WAV file
        //encodeMessageInWAV(targetFilePath, directoryPath, message, password);
        return "Encoded text file into WAV: " + targetFilePath;
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
        std::cerr << "Error: Input file does not exist." << std::endl;
        return;
    }
    fs::path inputPath(targetFilePath);
    fs::path outputImagePath = inputPath.parent_path() / (inputPath.stem().string() + "_encoded" + inputPath.extension().string());
    
    if (fileExtension == "png") {
        encodeFileInPNG(targetFilePath, outputImagePath, textFilePath, password);
        return "Encoded text file into PNG: " + targetFilePath;
    } else if (fileExtension == "wav") {
        // Add logic for encoding text content into a WAV file
        //encodeMessageInWAV(targetFilePath, directoryPath, message, password);
        return "Encoded text file into WAV: " + targetFilePath;
    } else {
        throw std::runtime_error("Unsupported file format: " + fileExtension);
    }
}

