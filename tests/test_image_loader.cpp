// test_image_loader.cpp
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

    RawImage image = ImageLoader::LoadRawImage(test_file, 2, 2);

    EXPECT_EQ(image.width, 2);
    EXPECT_EQ(image.height, 2);

    EXPECT_EQ(image.data.size(), 12);
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