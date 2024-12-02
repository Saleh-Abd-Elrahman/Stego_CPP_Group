// include/operations.h

#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>

// Encoding Functions
void encodeDirectMessage();
void encodeFile();
void encodePNGFile();

// Decoding Functions
void decodeMessage();
void decodeFile();
void decodePNGFile();

void runMenu();

// Helper Function
bool isValidPNG(const std::string& filePath);

#endif // OPERATIONS_H
