// negative_filter.h
#pragma once
#include <vector>
#include <cstdint>

struct NegativeFilter {
    static void Apply(std::vector<uint8_t>& rgb_data);
};