// color_filter.h
#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class ColorFilterType { None, Negative, Grayscale, PerlinNoise };

class ColorFilter {
public:
    static std::vector<uint8_t> Apply(const std::vector<uint8_t>& rgb_data, uint64_t width,
                                      uint64_t height, ColorFilterType filter_type,
                                      float perlin_noise_scale = -1.0f);

    static ColorFilterType Parse(const std::string& filter_name);
};