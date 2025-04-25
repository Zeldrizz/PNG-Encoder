// deflate.h
#pragma once

#include <vector>
#include <cstdint>

class DeflateCompressor {
public:
    static std::vector<uint8_t> Compress(const std::vector<uint8_t>& data);
};