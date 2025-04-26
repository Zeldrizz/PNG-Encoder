// perlin_noise_filter.cpp
#include "../include/perlin_noise_filter.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <random>
#include <numeric>
#include <vector>

namespace {

class Perlin2D {
public:
    explicit Perlin2D(uint32_t seed = 0xBADC0FFE) {
        p_.resize(256);
        std::iota(p_.begin(), p_.end(), 0);
        std::mt19937 gen(seed);
        std::shuffle(p_.begin(), p_.end(), gen);
        p_.insert(p_.end(), p_.begin(), p_.end());
    }

    float noise(float x, float y) const {
        int xi = static_cast<int>(std::floor(x)) & 255;
        int yi = static_cast<int>(std::floor(y)) & 255;
        float xf = x - std::floor(x);
        float yf = y - std::floor(y);

        int aa = p_[p_[xi] + yi];
        int ab = p_[p_[xi] + yi + 1];
        int ba = p_[p_[xi + 1] + yi];
        int bb = p_[p_[xi + 1] + yi + 1];

        float u = fade(xf);
        float v = fade(yf);

        float x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u);
        float x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);

        return lerp(x1, x2, v);
    }

private:
    static float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    static float grad(int h, float x, float y) {
        switch (h & 3) {
            case 0:
                return x + y;
            case 1:
                return -x + y;
            case 2:
                return x - y;
            default:
                return -x - y;
        }
    }

    std::vector<int> p_;
};

}  // namespace

void PerlinNoiseFilter::Apply(std::vector<uint8_t>& rgb_data, uint64_t width, uint64_t height,
                              float percent) {
    if (percent <= 0.0f || rgb_data.empty()) {
        return;
    }

    percent = std::clamp(percent, 0.0f, 100.0f);

    const float base_freq = 0.02f;
    const float max_extrafreq = 0.38f;
    const float frequency = base_freq + max_extrafreq * (percent / 100.0f);

    const float max_amplitude = 128.0f;
    const float amplitude = max_amplitude * (percent / 100.0f);

    Perlin2D perlin(0xC0FFEE);

    for (uint64_t y = 0; y < height; ++y) {
        for (uint64_t x = 0; x < width; ++x) {
            float n = perlin.noise(x * frequency, y * frequency);
            int delta = static_cast<int>(n * amplitude);

            size_t idx = (y * width + x) * 3;
            for (int c = 0; c < 3; ++c) {
                int new_val = static_cast<int>(rgb_data[idx + c]) + delta;
                rgb_data[idx + c] = static_cast<uint8_t>(std::clamp(new_val, 0, 255));
            }
        }
    }
}