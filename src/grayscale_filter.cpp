// grayscale_filter.cpp
#include "../include/grayscale_filter.h"
#include <algorithm>

void GrayscaleFilter::Apply(std::vector<uint8_t>& rgb_data) {
    const float red_weight = 0.299f;
    const float green_weight = 0.587f;
    const float blue_weight = 0.114f;

    for (size_t pixel_index = 0; pixel_index < rgb_data.size(); pixel_index += 3) {
        float red = rgb_data[pixel_index];
        float green = rgb_data[pixel_index + 1];
        float blue = rgb_data[pixel_index + 2];

        uint8_t gray_value = static_cast<uint8_t>(
            std::clamp(red_weight * red + green_weight * green + blue_weight * blue, 0.f, 255.f));

        rgb_data[pixel_index] = gray_value;
        rgb_data[pixel_index + 1] = gray_value;
        rgb_data[pixel_index + 2] = gray_value;
    }
}