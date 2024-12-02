#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

// Encrypts the plaintext using the Vigenère Cipher with the given key
std::string encryptVigenere(const std::string& plaintext, const std::string& key);

// Decrypts the ciphertext using the Vigenère Cipher with the given key
std::string decryptVigenere(const std::string& ciphertext, const std::string& key);

#endif // VIGENERE_H
