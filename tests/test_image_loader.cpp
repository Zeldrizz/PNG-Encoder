// test_image_loader.cpp
#include <gtest/gtest.h>
#include "image_loader.h"
#include <cstdint>
#include <fstream>
#include <cstdio>

// Verifies that a RAW file containing exactly width*height*3 bytes is
// loaded correctly and that pixel data matches the original byte sequence
TEST(ImageLoaderTest, LoadsRawDataCorrectly) {
    std::string test_file = "test_input.raw";

    {
        std::ofstream file(test_file, std::ios::binary);
        uint8_t data[12] = {
            255, 0,   0,    // Red
            0,   255, 0,    // Greeb
            0,   0,   255,  // Blue
            255, 255, 255   // White
        };
        file.write(reinterpret_cast<char*>(data), sizeof(data));
    }

    RawImage image = ImageLoader::LoadRawImage(test_file, 2, 2);

    EXPECT_EQ(image.width, 2);
    EXPECT_EQ(image.height, 2);
    EXPECT_EQ(image.data.size(), 12);

    // Check that the loaded data matches the bytes we wrote
    EXPECT_EQ(image.data[0], 255);
    EXPECT_EQ(image.data[1], 0);
    EXPECT_EQ(image.data[2], 0);

    EXPECT_EQ(image.data[3], 0);
    EXPECT_EQ(image.data[4], 255);
    EXPECT_EQ(image.data[5], 0);

    EXPECT_EQ(image.data[6], 0);
    EXPECT_EQ(image.data[7], 0);
    EXPECT_EQ(image.data[8], 255);

    EXPECT_EQ(image.data[9], 255);
    EXPECT_EQ(image.data[10], 255);
    EXPECT_EQ(image.data[11], 255);

    std::remove(test_file.c_str());
}

// Ensures that attempting to load a file which does not exist
// throws a std::runtime_error
TEST(ImageLoaderTest, ThrowsOnMissingFile) {
    uint64_t width = 1;
    uint64_t height = 1;
    EXPECT_THROW(ImageLoader::LoadRawImage("definitely_missing.raw", width, height),
                 std::runtime_error);
}

// Ensures that a RAW file shorter than the expected size (width*height*3)
// causes a std::runtime_error to be thrown
TEST(ImageLoaderTest, ThrowsOnShortFile) {
    const char* file_name = "short.raw";
    {
        std::ofstream f(file_name, std::ios::binary);
        uint8_t two_bytes[2] = {0, 0};
        f.write(reinterpret_cast<char*>(two_bytes), sizeof(two_bytes));
    }

    uint64_t width = 1;
    uint64_t height = 1;
    EXPECT_THROW(ImageLoader::LoadRawImage(file_name, width, height), std::runtime_error);
    std::remove(file_name);
}

// Confirms that if the RAW file contains more bytes than needed,
// LoadRawImage reads only width*height*3 bytes and ignores the rest
TEST(ImageLoaderTest, IgnoresExtraBytes) {
    const char* file_name = "long.raw";
    {
        std::ofstream f(file_name, std::ios::binary);
        std::vector<uint8_t> bytes(6, 42);  // 6 bytes, but only 3 are needed
        f.write(reinterpret_cast<char*>(bytes.data()), bytes.size());
    }

    uint64_t width = 1;
    uint64_t height = 1;
    RawImage img = ImageLoader::LoadRawImage(file_name, width, height);

    EXPECT_EQ(img.data.size(), 3u);
    EXPECT_EQ(img.data[0], 42u);
    std::remove(file_name);
}