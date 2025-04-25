// perlin_noise_filter.h
#pragma once
#include <vector>
#include <cstdint>

struct PerlinNoiseFilter {
    static void Apply(std::vector<uint8_t>& rgb_data, uint64_t width, uint64_t height,
                      float noise_scale = -1.f);
};