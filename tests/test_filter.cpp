// test_filter.cpp
#include <gtest/gtest.h>
#include "filter.h"
#include <cstdint>
#include <vector>

// Applying the Paeth filter on a uniform 2×2 RGB image
// 1) Verifies output buffer size: (width*channels + 1) * height
// 2) Checks that each row starts with the Paeth filter type byte
// 3) Ensures first pixel remains unfiltered, and all other filtered deltas are zero
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

// Validates the Paeth predictor logic by constructing
// specific A, B, C and current values
// 1) Uses a local PaethPredictor implementation to compute expected delta
// 2) Compares that against the single-channel output from Apply
TEST(FilterTest, PaethBehaviourViaApply) {
    auto PaethPredictorLocal = [](uint8_t a, uint8_t b, uint8_t c) -> uint8_t {
        int p = int(a) + int(b) - int(c);
        int pa = std::abs(p - int(a));
        int pb = std::abs(p - int(b));
        int pc = std::abs(p - int(c));
        return (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c);
    };

    struct Case {
        uint8_t A;
        uint8_t B;
        uint8_t C;
        uint8_t cur;
    };
    std::vector<Case> cases = {{10, 20, 0, 25}, {50, 5, 50, 60}, {200, 100, 150, 180}};

    uint64_t width = 2;
    uint64_t height = 2;
    size_t row_sz = width * 3 + 1;

    for (const auto& t : cases) {
        std::vector<uint8_t> data(width * height * 3, 0);

        data[(0 * width + 0) * 3 + 0] = t.C;
        data[(0 * width + 1) * 3 + 0] = t.B;
        data[(1 * width + 0) * 3 + 0] = t.A;
        data[(1 * width + 1) * 3 + 0] = t.cur;

        auto filtered = PNGFilter::Apply(data, width, height);

        size_t row_start = row_sz * 1;
        uint8_t actual = filtered[row_start + 1 + 3];

        uint8_t predictor = PaethPredictorLocal(t.A, t.B, t.C);
        uint8_t expected = static_cast<uint8_t>(t.cur - predictor);

        EXPECT_EQ(actual, expected)
            << "A=" << int(t.A) << " B=" << int(t.B) << " C=" << int(t.C) << " cur=" << int(t.cur);
    }
}

// Edge case with a single-pixel image (1×1)
// 1) Ensures the filter byte is correctly set
// 2) Checks that the lone pixel is output as raw value (wrap-around scenario)
TEST(FilterTest, ApplyPaethFilterWrapAround) {
    uint64_t width = 1;
    uint64_t height = 1;
    std::vector<uint8_t> data = {5, 5, 5};

    auto filtered = PNGFilter::Apply(data, width, height);

    EXPECT_EQ(filtered[0], static_cast<uint8_t>(PNGFilterType::Paeth));

    EXPECT_EQ(filtered[1], 5u);
}