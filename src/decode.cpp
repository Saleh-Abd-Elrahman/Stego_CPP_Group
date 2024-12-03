#include "decode.h"
#include "file_utils.h"
#include <filesystem>
#include "decoder_PNG.h"

// Decode the file and return the hidden message
std::string decodeMessage(const std::string& filePath, const std::string& password) {
    std::string fileExtension = getFileExtension(filePath);
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    namespace fs = std::filesystem;

    if (!fs::exists(filePath)) {
        return "Error: Input file does not exist.";
    }

    fs::path inputPath(filePath);
    fs::path outputImagePath = inputPath.parent_path() / (inputPath.stem().string() + "_encoded" + inputPath.extension().string());

    if (fileExtension == "png") {
        return decodeMessageFromPNG(filePath, password);
    } else if (fileExtension == "wav") {
        // Add logic for encoding text content into a WAV file
        //encodeMessageInWAV(targetFilePath, directoryPath, message, password);
        return "Encoded text file into WAV: " + filePath;
    } else {
        throw std::runtime_error("Unsupported file format: " + fileExtension);
    }
}

// Decode the file and return the hidden file
std::string decodeFile(const std::string& filePath, const std::string& password) {
    std::string fileExtension = getFileExtension(filePath);
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    namespace fs = std::filesystem;

    if (!fs::exists(filePath)) {
        return "Error: Input file does not exist.";
    }

    fs::path inputPath(filePath);
    fs::path outputFilePath = inputPath.parent_path() / (inputPath.stem().string() + "_decoded.txt");

    if (fileExtension == "png") {
        if (decodeFileFromPNG(filePath, outputFilePath, password)) {
            return "Decoded text file saved to: " + outputFilePath.string();
        } else {
            return "Failed to decode text file.";
        }
    } else if (fileExtension == "wav") {
        // Add logic for encoding text content into a WAV file
        //encodeMessageInWAV(targetFilePath, directoryPath, message, password);
        return "Decoded text file saved to: " + filePath;
    } else {
        throw std::runtime_error("Unsupported file format: " + fileExtension);
    }

}


// Decode the file and return the hidden file
std::string decodeBashScript(const std::string& filePath, const std::string& password) {
    std::string fileExtension = getFileExtension(filePath);
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    namespace fs = std::filesystem;

    if (!fs::exists(filePath)) {
        return "Error: Input file does not exist.";
    }

    fs::path inputPath(filePath);
    fs::path outputFilePath = inputPath.parent_path() / (inputPath.stem().string() + "_decoded.sh");

    if (fileExtension == "png") {
        if (decodeAndExecuteScript(filePath, outputFilePath, password)) {
            return "Decoded bash script saved to: " + outputFilePath.string() + " and executed successfully.";
        } else {
            return "Failed to decode bash scipt.";
        }
    } else if (fileExtension == "wav") {
        // Add logic for encoding text content into a WAV file
        //encodeMessageInWAV(targetFilePath, directoryPath, message, password);
        return "Decoded bash script saved to: " + filePath;
    } else {
        throw std::runtime_error("Unsupported file format: " + fileExtension);
    }

}