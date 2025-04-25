// negative_filter.cpp
#include "../include/negative_filter.h"

void NegativeFilter::Apply(std::vector<uint8_t>& rgb_data) {
    for (auto& pixel_value : rgb_data) {
        pixel_value = 255 - pixel_value;
    }
}