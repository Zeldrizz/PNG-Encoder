// filter.h
#pragma once

#include <cstdint>
#include <vector>

enum class PNGFilterType { None = 0, Sub = 1, Up = 2, Average = 3, Paeth = 4 };

class PNGFilter {
public:
    static std::vector<uint8_t> Apply(const std::vector<uint8_t>& rgb_data, uint64_t width,
                                      uint64_t height);

private:
    static uint8_t PaethPredictor(uint8_t a, uint8_t b, uint8_t c);
};