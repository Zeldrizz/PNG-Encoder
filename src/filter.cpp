#include "../include/filter.h"
#include <cmath>

uint8_t PNGFilter::PaethPredictor(uint8_t a, uint8_t b, uint8_t c) {
    int predict = static_cast<int>(a) + static_cast<int>(b) - static_cast<int>(c);

    int predict_a = std::abs(predict - static_cast<int>(a));
    int predict_b = std::abs(predict - static_cast<int>(b));
    int predict_c = std::abs(predict - static_cast<int>(c));

    if (predict_a <= predict_b && predict_a <= predict_c) {
        return a;
    } else if (predict_b <= predict_c) {
        return b;
    }

    return c;
}

std::vector<uint8_t> PNGFilter::Apply(const std::vector<uint8_t>& rgb_data, uint64_t width,
                                      uint64_t height) {
    std::vector<uint8_t> filtered;
    filtered.reserve((width * 3 + 1) * height);

    for (size_t y = 0; y < height; ++y) {
        filtered.push_back(static_cast<uint8_t>(PNGFilterType::Paeth));

        for (size_t x = 0; x < width; ++x) {
            for (size_t c = 0; c < 3; ++c) {
                uint8_t A = 0;
                uint8_t B = 0;
                uint8_t C = 0;

                if (x > 0) {
                    A = rgb_data[(y * width + (x - 1)) * 3 + c];
                }

                if (y > 0) {
                    B = rgb_data[((y - 1) * width + x) * 3 + c];
                }

                if (x > 0 && y > 0) {
                    C = rgb_data[((y - 1) * width + (x - 1)) * 3 + c];
                }

                uint8_t current = rgb_data[(y * width + x) * 3 + c];
                uint8_t pred = PaethPredictor(A, B, C);
                uint8_t filtered_val = current - pred;
                filtered.push_back(filtered_val);
            }
        }
    }

    return filtered;
}