// test_color_filters.cpp
#include <gtest/gtest.h>
#include "negative_filter.h"
#include "grayscale_filter.h"
#include "perlin_noise_filter.h"
#include <cstdint>
#include <vector>
#include <algorithm>

// NegativeFilter
// Verifies each byte is transformed to 255 - value
TEST(NegativeFilterTest, ApplyInvertsColors) {
    std::vector<uint8_t> data = {0, 127, 255, 10, 200, 30};

    std::vector<uint8_t> expected = {255, 128, 0, 245, 55, 225};

    NegativeFilter::Apply(data);

    EXPECT_EQ(data, expected);
}

// NegativeFilter
// Verifies that applying the filter twice returns the original data
TEST(NegativeFilterTest, DoubleApplyRestoresOriginal) {
    std::vector<uint8_t> original = {42, 17, 221, 128, 64, 32};
    std::vector<uint8_t> data = original;

    NegativeFilter::Apply(data);
    NegativeFilter::Apply(data);

    EXPECT_EQ(data, original);
}

// GrayscaleFilter
// All three channels of the pixel equal the resulting gray value
TEST(GrayscaleFilterTest, ConvertsPrimaryColors) {
    std::vector<uint8_t> data = {255, 0, 0, 0, 255, 0, 0, 0, 255};

    GrayscaleFilter::Apply(data);

    ASSERT_EQ(data.size(), 9u);

    EXPECT_EQ(data[0], 76u);
    EXPECT_EQ(data[1], 76u);
    EXPECT_EQ(data[2], 76u);

    EXPECT_EQ(data[3], 149u);
    EXPECT_EQ(data[4], 149u);
    EXPECT_EQ(data[5], 149u);

    EXPECT_EQ(data[6], 29u);
    EXPECT_EQ(data[7], 29u);
    EXPECT_EQ(data[8], 29u);
}

// GrayscaleFilter
// Pixels where R == G == B remain unchanged
TEST(GrayscaleFilterTest, KeepsGrayPixelsUnchanged) {
    std::vector<uint8_t> original = {50, 50, 50, 200, 200, 200};
    std::vector<uint8_t> data = original;

    GrayscaleFilter::Apply(data);

    EXPECT_EQ(data, original);
}

// PerlinNoiseFilter
// percent == 0 -> buffer remains unchanged
TEST(PerlinNoiseFilterTest, NoEffectWhenPercentZero) {
    uint64_t width = 2;
    uint64_t height = 2;
    std::vector<uint8_t> original(width * height * 3, 123);
    std::vector<uint8_t> data = original;

    PerlinNoiseFilter::Apply(data, width, height, 0.0f);

    EXPECT_EQ(data, original);
}

// PerlinNoiseFilter
// Two runs with the same parameters produce identical results
// All bytes stay within the range [0, 255]
// At least one byte changes from the original
TEST(PerlinNoiseFilterTest, DeterministicAndInRange) {
    uint64_t width = 2;
    uint64_t height = 2;
    std::vector<uint8_t> base(width * height * 3, 100);

    std::vector<uint8_t> out1 = base;
    PerlinNoiseFilter::Apply(out1, width, height, 75.0f);

    std::vector<uint8_t> out2 = base;
    PerlinNoiseFilter::Apply(out2, width, height, 75.0f);

    EXPECT_EQ(out1, out2);

    for (uint8_t v : out1) {
        EXPECT_GE(v, 0u);
        EXPECT_LE(v, 255u);
    }

    EXPECT_NE(out1, base);
}