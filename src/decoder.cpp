#include "decoder.h"
#include <jpeglib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

bool decodeFileFromJPEG(const std::string& imagePath, const std::string& outputFilePath) {
    // Open input JPEG file
    FILE* infile = fopen(imagePath.c_str(), "rb");
    if (!infile) {
        std::cerr << "Error: Unable to open file " << imagePath << std::endl;
        return false;
    }

    // Initialize the JPEG decompression object
    struct jpeg_decompress_struct srcinfo;
    struct jpeg_error_mgr jsrcerr;
    srcinfo.err = jpeg_std_error(&jsrcerr);
    jpeg_create_decompress(&srcinfo);
    jpeg_stdio_src(&srcinfo, infile);
    jpeg_read_header(&srcinfo, TRUE);

    // Read the source file's coefficient arrays
    jvirt_barray_ptr* coef_arrays = jpeg_read_coefficients(&srcinfo);

    // Extract the message
    std::vector<bool> messageBits;
    size_t messageLength = 0;
    size_t bitsRead = 0;
    bool lengthExtracted = false;

    for (int compNum = 0; compNum < srcinfo.num_components; ++compNum) {
        jpeg_component_info* compptr = srcinfo.comp_info + compNum;
        JBLOCKARRAY coef_buffers = (srcinfo.mem->access_virt_barray)(
            (j_common_ptr)&srcinfo, coef_arrays[compNum], 0, compptr->height_in_blocks, FALSE);

        for (JDIMENSION row = 0; row < compptr->height_in_blocks; ++row) {
            JBLOCKROW buffer = coef_buffers[row];
            for (JDIMENSION col = 0; col < compptr->width_in_blocks; ++col) {
                JCOEFPTR block = buffer[col];

                for (int i = 1; i < DCTSIZE2; ++i) {
                    if (block[i] != 0) {
                        bool bit = block[i] & 1;
                        messageBits.push_back(bit);
                        bitsRead++;

                        if (!lengthExtracted && bitsRead == 32) {
                            // Extract message length
                            messageLength = 0;
                            for (size_t j = 0; j < 32; ++j) {
                                messageLength = (messageLength << 1) | messageBits[j];
                            }
                            messageBits.clear();
                            bitsRead = 0;
                            lengthExtracted = true;

                            // Optional: Check for unreasonable message length
                            if (messageLength > (srcinfo.image_width * srcinfo.image_height * 3)) {
                                std::cerr << "Error: Extracted message length is unreasonably large." << std::endl;
                                jpeg_finish_decompress(&srcinfo);
                                jpeg_destroy_decompress(&srcinfo);
                                fclose(infile);
                                return false;
                            }
                        }

                        if (lengthExtracted && bitsRead == messageLength * 8) {
                            goto END_EXTRACTION;
                        }
                    }
                }
            }
        }
    }

END_EXTRACTION:

    if (!lengthExtracted) {
        std::cerr << "Error: Failed to extract message length." << std::endl;
        jpeg_finish_decompress(&srcinfo);
        jpeg_destroy_decompress(&srcinfo);
        fclose(infile);
        return false;
    }

    // Convert bits to bytes
    std::vector<unsigned char> messageData;
    for (size_t i = 0; i < messageBits.size(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; ++j) {
            byte = (byte << 1) | messageBits[i + j];
        }
        messageData.push_back(byte);
    }

    // Write the decoded message to the output file
    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Unable to open output file " << outputFilePath << std::endl;
        jpeg_finish_decompress(&srcinfo);
        jpeg_destroy_decompress(&srcinfo);
        fclose(infile);
        return false;
    }
    outFile.write(reinterpret_cast<char*>(messageData.data()), messageData.size());
    outFile.close();

    // Finish decompression
    jpeg_finish_decompress(&srcinfo);
    jpeg_destroy_decompress(&srcinfo);

    fclose(infile);

    return true;
}
