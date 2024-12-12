#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct RawImage {
    uint64_t width;
    uint64_t height;

    std::vector<uint8_t> data;
};

class ImageLoader {
public:
    static RawImage LoadRawImage(const std::string &path, uint64_t width, uint64_t height);
};