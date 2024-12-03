#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

/**
 * @file vigenere.h
 * @brief Provides functions for Vigenère Cipher encryption and decryption.
 *
 * The Vigenère Cipher is used to encrypt and decrypt passwords, securing the hidden data within audio files.
 */

/**
 * @brief Encrypts plaintext using the Vigenère Cipher with the provided key.
 *
 * This function scrambles the plaintext by shifting each character based on the corresponding character in the key.
 * The key is repeated to match the length of the plaintext.
 *
 * @param plaintext The text to be encrypted.
 * @param key The encryption key. It should be at least as long as the plaintext for optimal security.
 * @return A `std::string` containing the encrypted ciphertext.
 */
std::string encryptVigenere(const std::string& plaintext, const std::string& key);

/**
 * @brief Decrypts ciphertext using the Vigenère Cipher with the provided key.
 *
 * This function unscrambles the ciphertext by shifting each character back based on the corresponding character in the key.
 * The key is repeated to match the length of the ciphertext.
 *
 * @param ciphertext The text to be decrypted.
 * @param key The decryption key. It must be the same as the encryption key used to encrypt the plaintext.
 * @return A `std::string` containing the decrypted plaintext.
 */
std::string decryptVigenere(const std::string& ciphertext, const std::string& key);

#endif // VIGENERE_H
