#include <gtest/gtest.h>
#include "image_loader.h"
#include <cstdint>
#include <fstream>
#include <cstdio>

TEST(ImageLoaderTest, LoadsRawDataCorrectly) {
    std::string test_file = "test_input.raw";

    {
        std::ofstream file(test_file, std::ios::binary);
        uint8_t data[12] = {
            255, 0,   0,    // Красный
            0,   255, 0,    // Зеленый
            0,   0,   255,  // Синий
            255, 255, 255   // Белый
        };
        file.write(reinterpret_cast<char*>(data), sizeof(data));
    }

    RawImage img = ImageLoader::LoadRawImage(test_file, 2, 2);

    EXPECT_EQ(img.width, 2);
    EXPECT_EQ(img.height, 2);

    EXPECT_EQ(img.data.size(), 12);
    EXPECT_EQ(img.data[0], 255);
    EXPECT_EQ(img.data[1], 0);
    EXPECT_EQ(img.data[2], 0);

    EXPECT_EQ(img.data[3], 0);
    EXPECT_EQ(img.data[4], 255);
    EXPECT_EQ(img.data[5], 0);

    EXPECT_EQ(img.data[6], 0);
    EXPECT_EQ(img.data[7], 0);
    EXPECT_EQ(img.data[8], 255);

    EXPECT_EQ(img.data[9], 255);
    EXPECT_EQ(img.data[10], 255);
    EXPECT_EQ(img.data[11], 255);

    std::remove(test_file.c_str());
}