#pragma once

#include <cstdint>
#include <vector>
#include <string>

class PNGWriter {
public:
    static void WritePNG(const std::string& filename, uint64_t width, uint64_t height, const std::vector<uint8_t>& compressed_data);
};