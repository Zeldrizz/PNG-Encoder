// color_filter.cpp
#include "../include/color_filter.h"
#include "../include/negative_filter.h"
#include "../include/grayscale_filter.h"
#include "../include/perlin_noise_filter.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

ColorFilterType ColorFilter::Parse(const std::string& filter_name) {
    std::string lower_name;
    lower_name.reserve(filter_name.size());
    std::transform(filter_name.begin(), filter_name.end(), std::back_inserter(lower_name),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

    if (lower_name == "negative") {
        return ColorFilterType::Negative;
    }

    if (lower_name == "grayscale") {
        return ColorFilterType::Grayscale;
    }

    if (lower_name == "perlin") {
        return ColorFilterType::PerlinNoise;
    }

    if (lower_name == "none") {
        return ColorFilterType::None;
    }

    throw std::runtime_error("Unknown color filter: " + filter_name);
}

std::vector<uint8_t> ColorFilter::Apply(const std::vector<uint8_t>& rgb_data, uint64_t width,
                                        uint64_t height, ColorFilterType filter_type,
                                        float perlin_noise_scale) {
    if (filter_type == ColorFilterType::None) {
        return rgb_data;
    }

    std::vector<uint8_t> output_data = rgb_data;
    switch (filter_type) {
        case ColorFilterType::Negative:
            NegativeFilter::Apply(output_data);
            break;
        case ColorFilterType::Grayscale:
            GrayscaleFilter::Apply(output_data);
            break;
        case ColorFilterType::PerlinNoise:
            PerlinNoiseFilter::Apply(output_data, width, height, perlin_noise_scale);
            break;
        default:
            break;
    }

    return output_data;
}