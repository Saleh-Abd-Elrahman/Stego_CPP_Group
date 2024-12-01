#include "encoder.h"
#include <jpeglib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

bool encodeFileInJPEG(const std::string& inputImagePath, const std::string& outputImagePath, const std::string& inputFilePath) {
    // Read the content of the input file
    std::ifstream inFile(inputFilePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: Unable to open file " << inputFilePath << std::endl;
        return false;
    }
    std::vector<unsigned char> fileContent((std::istreambuf_iterator<char>(inFile)),
                                            std::istreambuf_iterator<char>());
    inFile.close();

    // Open input JPEG file
    FILE* infile = fopen(inputImagePath.c_str(), "rb");
    if (!infile) {
        std::cerr << "Error: Unable to open input file " << inputImagePath << std::endl;
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
    jvirt_barray_ptr* src_coef_arrays = jpeg_read_coefficients(&srcinfo);

    // Open output JPEG file
    FILE* outfile = fopen(outputImagePath.c_str(), "wb");
    if (!outfile) {
        std::cerr << "Error: Unable to open output file " << outputImagePath << std::endl;
        jpeg_destroy_decompress(&srcinfo);
        fclose(infile);
        return false;
    }

    // Initialize the JPEG compression object
    struct jpeg_compress_struct dstinfo;
    struct jpeg_error_mgr jdsterr;
    dstinfo.err = jpeg_std_error(&jdsterr);
    jpeg_create_compress(&dstinfo);
    jpeg_stdio_dest(&dstinfo, outfile);

    // Copy critical parameters from source to destination
    jpeg_copy_critical_parameters(&srcinfo, &dstinfo);

    // Modify the DCT coefficients in src_coef_arrays before writing

    // Prepare the message
    uint32_t messageLength = fileContent.size();
    std::vector<bool> messageBits;

    // Message length to bits (32 bits)
    for (int i = 31; i >= 0; --i) {
        messageBits.push_back((messageLength >> i) & 1);
    }

    // Message content to bits
    for (unsigned char c : fileContent) {
        for (int i = 7; i >= 0; --i) {
            messageBits.push_back((c >> i) & 1);
        }
    }

    // Calculate capacity
    size_t maxCapacity = 0;
    for (int compNum = 0; compNum < srcinfo.num_components; ++compNum) {
        jpeg_component_info* compptr = srcinfo.comp_info + compNum;
        size_t blocks = compptr->width_in_blocks * compptr->height_in_blocks;
        maxCapacity += blocks * (DCTSIZE2 - 1); // Exclude DC coefficient
    }

    if (messageBits.size() > maxCapacity) {
        std::cerr << "Error: The image is too small to hold the entire message." << std::endl;
        jpeg_destroy_compress(&dstinfo);
        jpeg_destroy_decompress(&srcinfo);
        fclose(infile);
        fclose(outfile);
        return false;
    }

    // Embed the message
    size_t bitIndex = 0;
    bool dataComplete = false;

    for (int compNum = 0; compNum < srcinfo.num_components && !dataComplete; ++compNum) {
        jpeg_component_info* compptr = srcinfo.comp_info + compNum;
        JBLOCKARRAY coef_buffers = (srcinfo.mem->access_virt_barray)(
            (j_common_ptr)&srcinfo, src_coef_arrays[compNum], 0, compptr->height_in_blocks, TRUE);

        for (JDIMENSION row = 0; row < compptr->height_in_blocks && !dataComplete; ++row) {
            JBLOCKROW buffer = coef_buffers[row];
            for (JDIMENSION col = 0; col < compptr->width_in_blocks && !dataComplete; ++col) {
                JCOEFPTR block = buffer[col];

                for (int i = 1; i < DCTSIZE2 && !dataComplete; ++i) {
                    if (block[i] != 0) {
                        if (bitIndex < messageBits.size()) {
                            block[i] &= ~1; // Clear LSB
                            block[i] |= messageBits[bitIndex++] ? 1 : 0; // Set LSB
                        } else {
                            dataComplete = true;
                        }
                    }
                }
            }
        }
    }

    // Write the modified coefficients to the destination file
    jpeg_write_coefficients(&dstinfo, src_coef_arrays);

    // Finish compression
    jpeg_finish_compress(&dstinfo);
    jpeg_destroy_compress(&dstinfo);

    // Finish decompression
    jpeg_finish_decompress(&srcinfo);
    jpeg_destroy_decompress(&srcinfo);

    fclose(infile);
    fclose(outfile);

    return true;
}
