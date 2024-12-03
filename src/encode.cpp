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
        return "Error: Input file does not exist.";
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
        return "Error: Input file does not exist.";
    }
    fs::path inputPath(targetFilePath);
    fs::path outputImagePath = inputPath.parent_path() / (inputPath.stem().string() + "_encoded" + inputPath.extension().string());
    
    if (fileExtension == "png") {
        // Read the content of the input file in binary mode
        std::ifstream inFile(targetFilePath, std::ios::binary);
        if (!inFile) {
            return "Error: Unable to open file " + targetFilePath;
        }
        std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
        encodeMessageInPNG(targetFilePath, outputImagePath, fileContent, password);

        return "Encoded text file into PNG: " + targetFilePath;
    } else if (fileExtension == "wav") {
        // Add logic for encoding text content into a WAV file
        //encodeMessageInWAV(targetFilePath, directoryPath, message, password);
        return "Encoded text file into WAV: " + targetFilePath;
    } else {
        throw std::runtime_error("Unsupported file format: " + fileExtension);
    }
}

