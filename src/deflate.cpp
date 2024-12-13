#include "../include/deflate.h"
#include <zlib.h>
#include <stdexcept>

std::vector<uint8_t> DeflateCompressor::Compress(const std::vector<uint8_t>& data) {
    uLongf compressed_size = compressBound(data.size());
    std::vector<uint8_t> compressed_data(compressed_size);

    int ret = ::compress2(compressed_data.data(), &compressed_size, data.data(), data.size(), Z_BEST_COMPRESSION);

    if (ret != Z_OK) {
        throw std::runtime_error("Failed to compress data with zlib");
    }

    compressed_data.resize(compressed_size);
    return compressed_data;
}