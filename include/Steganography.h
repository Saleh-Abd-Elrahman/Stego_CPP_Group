#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <string>

/**
 * @class Steganography
 * @brief The Steganography class provides functionalities to encode and decode messages and files into/from WAV audio files with password protection.
 *
 * This class supports encoding and decoding of arbitrary files, including text, PNG images, and Bash scripts.
 * It utilizes the Vigenère Cipher for password-based encryption to secure the hidden data.
 */
class Steganography {
public:
    /**
     * @brief Encodes a textual message into a WAV audio file with password protection.
     *
     * This method embeds the provided message into the least significant bits (LSBs) of the input WAV file's audio samples.
     * The message is secured using an 8-character password through the Vigenère Cipher.
     *
     * @param inputWav Path to the input WAV file.
     * @param outputWav Path where the output WAV file with the encoded message will be saved.
     * @param message The textual message to be hidden within the audio file.
     * @param password An 8-character password used to encrypt the message.
     * @return `true` if encoding is successful; `false` otherwise.
     */
    static bool encodeMessage(const std::string& inputWav, 
                              const std::string& outputWav, 
                              const std::string& message, 
                              const std::string& password);

    /**
     * @brief Decodes a textual message from a WAV audio file after verifying the password.
     *
     * This method extracts the hidden message from the least significant bits (LSBs) of the WAV file's audio samples.
     * It verifies the provided password by decrypting the embedded scrambled password using the Vigenère Cipher.
     *
     * @param inputWav Path to the WAV file containing the encoded message.
     * @param password The 8-character password used during encoding to decrypt the message.
     * @return The decoded message as a `std::string` if successful; an empty string otherwise.
     */
    static std::string decodeMessage(const std::string& inputWav, 
                                     const std::string& password);

    /**
     * @brief Encodes an arbitrary file (text, PNG, Bash script) into a WAV audio file with password protection.
     *
     * This method embeds the contents of the specified file into the least significant bits (LSBs) of the input WAV file's audio samples.
     * The file data is secured using an 8-character password through the Vigenère Cipher.
     *
     * @param inputWav Path to the input WAV file.
     * @param outputWav Path where the output WAV file with the encoded file will be saved.
     * @param inputFilePath Path to the file to be hidden within the audio file.
     * @param password An 8-character password used to encrypt the file data.
     * @return `true` if encoding is successful; `false` otherwise.
     */
    static bool encodeFile(const std::string& inputWav, 
                           const std::string& outputWav, 
                           const std::string& inputFilePath, 
                           const std::string& password);

    /**
     * @brief Decodes an arbitrary file from a WAV audio file after verifying the password.
     *
     * This method extracts the hidden file from the least significant bits (LSBs) of the WAV file's audio samples.
     * It verifies the provided password by decrypting the embedded scrambled password using the Vigenère Cipher.
     * If the decoded file is a Bash script, it is executed automatically.
     *
     * @param inputWav Path to the WAV file containing the encoded file.
     * @param outputFilePath Path where the decoded file will be saved.
     * @param password The 8-character password used during encoding to decrypt the file data.
     * @return `true` if decoding is successful; `false` otherwise.
     */
    static bool decodeFile(const std::string& inputWav, 
                           const std::string& outputFilePath, 
                           const std::string& password);

private:
    /**
     * @brief Converts a textual message into its binary representation.
     *
     * @param message The message to convert.
     * @return A `std::string` containing the binary representation of the message.
     */
    static std::string messageToBinary(const std::string& message);

    /**
     * @brief Converts binary data back into a textual message.
     *
     * @param binaryData The binary data to convert.
     * @return The decoded message as a `std::string`.
     */
    static std::string binaryToMessage(const std::string& binaryData);

    /**
     * @brief Converts the contents of a file into its binary representation.
     *
     * @param fileData The data of the file to convert.
     * @return A `std::string` containing the binary representation of the file data.
     */
    static std::string fileToBinary(const std::string& fileData);

    /**
     * @brief Converts binary data back into its original file format.
     *
     * @param binaryData The binary data to convert.
     * @param outputFilePath The path where the decoded file will be saved.
     * @return `true` if the conversion and file writing are successful; `false` otherwise.
     */
    static bool binaryToFile(const std::string& binaryData, const std::string& outputFilePath);

    /**
     * @brief Permanently erases the hidden data from a WAV audio file by clearing the LSBs.
     *
     * This method overwrites the least significant bits (LSBs) of the audio samples with `0`,
     * effectively removing any embedded hidden data.
     *
     * @param wavPath Path to the WAV file from which to erase hidden data.
     * @return `true` if data erasure is successful; `false` otherwise.
     */
    static bool eraseHiddenData(const std::string& wavPath);
};

#endif // STEGANOGRAPHY_H
