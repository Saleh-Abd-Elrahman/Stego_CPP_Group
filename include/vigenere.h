#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

/**
 * @brief Encrypts plaintext using the Vigenère Cipher with the provided key.
 *
 * This function encrypts the input plaintext by shifting each character based on the corresponding
 * character in the repeated key. The key is repeated or truncated to match the length of the plaintext.
 *
 * @param plaintext The text string to be encrypted.
 * @param key The cipher key used for encryption. It should be non-empty.
 * @return The encrypted ciphertext as a string.
 */
std::string encryptVigenere(const std::string& plaintext, const std::string& key);

/**
 * @brief Decrypts ciphertext using the Vigenère Cipher with the provided key.
 *
 * This function decrypts the input ciphertext by reversing the shifting process based on the
 * corresponding character in the repeated key. The key is repeated or truncated to match the length
 * of the ciphertext.
 *
 * @param ciphertext The encrypted text string to be decrypted.
 * @param key The cipher key used for decryption. It should be the same key used during encryption.
 * @return The decrypted plaintext as a string.
 */
std::string decryptVigenere(const std::string& ciphertext, const std::string& key);

#endif // VIGENERE_H
