// test_filter.cpp
#include <gtest/gtest.h>
#include "filter.h"
#include <cstdint>
#include <vector>

TEST(FilterTest, ApplyPaethFilter) {
    uint64_t width = 2;
    uint64_t height = 2;
    std::vector<uint8_t> data(width * height * 3, 100);

    auto filtered = PNGFilter::Apply(data, width, height);

    EXPECT_EQ(filtered.size(), static_cast<size_t>((width * 3 + 1) * height));

    for (uint64_t y = 0; y < height; ++y) {
        size_t row_start = y * (width * 3 + 1);

        EXPECT_EQ(filtered[row_start], static_cast<uint8_t>(PNGFilterType::Paeth))
            << "Incorrect filter type at row " << y;

        for (uint64_t x = 0; x < width; ++x) {
            for (size_t c = 0; c < 3; ++c) {
                size_t byte_index = row_start + 1 + x * 3 + c;

                if (y == 0 && x == 0) {
                    EXPECT_EQ(filtered[byte_index], 100)
                        << "First data byte in first row incorrect at byte index " << byte_index;
                } else {
                    EXPECT_EQ(filtered[byte_index], 0) << "Filtered byte incorrect at row " << y
                                                       << ", pixel " << x << ", channel " << c;
                }
            }
        }
    }
}