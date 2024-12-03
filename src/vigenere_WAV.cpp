#include "vigenere_WAV.h"

// Helper function to repeat the key to match the length of the text
std::string repeatKey(const std::string& key, size_t length) {
    std::string repeated;
    while (repeated.size() < length) {
        repeated += key;
    }
    return repeated.substr(0, length);
}

std::string encryptVigenere(const std::string& plaintext, const std::string& key) {
    std::string encrypted;
    std::string extendedKey = repeatKey(key, plaintext.size());

    for (size_t i = 0; i < plaintext.size(); ++i) {
        char p = plaintext[i];
        char k = extendedKey[i];
        encrypted += static_cast<char>((static_cast<unsigned char>(p) + static_cast<unsigned char>(k)) % 256);
    }

    return encrypted;
}

std::string decryptVigenere(const std::string& ciphertext, const std::string& key) {
    std::string decrypted;
    std::string extendedKey = repeatKey(key, ciphertext.size());

    for (size_t i = 0; i < ciphertext.size(); ++i) {
        char c = ciphertext[i];
        char k = extendedKey[i];
        decrypted += static_cast<char>((static_cast<unsigned char>(c) - static_cast<unsigned char>(k) + 256) % 256);
    }

    return decrypted;
}
