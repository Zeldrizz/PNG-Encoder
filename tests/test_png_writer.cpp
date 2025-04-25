// test_png_writer.cpp
#include <gtest/gtest.h>
#include "png_writer.h"
#include "filter.h"
#include "deflate.h"
#include <fstream>
#include <cstdio>

namespace {

// Helper to read exactly n bytes into buffer; returns true on success
bool ReadBytes(std::ifstream& in, std::vector<uint8_t>& buffer, size_t n) {
    buffer.resize(n);
    in.read(reinterpret_cast<char*>(buffer.data()), n);
    return static_cast<size_t>(in.gcount()) == n;
}

// Read big-endian 32-bit integer from 4 bytes
uint32_t ReadBE32(const uint8_t* p) {
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}

}  // namespace

// Write a minimal 1×1 truecolor PNG and verify its structure:
// 1) Correct PNG signature
// 2) IHDR chunk length, type, and fields (width, height, bit depth, color type)
// 3) IDAT chunk length matches compressed data size, correct type
// 4) Presence of IEND chunk with zero-length data
TEST(PNGWriterTest, WritesMinimalPNG) {
    uint64_t width = 1;
    uint64_t height = 1;
    const std::string file_name = "tiny.png";

    // Prepare a single red pixel, filter and compress it
    std::vector<uint8_t> raw = {255, 0, 0};
    auto filtered = PNGFilter::Apply(raw, width, height);
    auto compressed = DeflateCompressor::Compress(filtered);

    PNGWriter writer;
    ASSERT_NO_THROW(writer.WritePNG(file_name, width, height, compressed));

    std::ifstream in(file_name, std::ios::binary);
    ASSERT_TRUE(in) << "File wasn't created!";

    // Check PNG signature 
    std::vector<uint8_t> buffer;
    ASSERT_TRUE(ReadBytes(in, buffer, 8));
    const uint8_t ref_sig[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'};
    EXPECT_EQ(std::vector<uint8_t>(ref_sig, ref_sig + 8), buffer);

    // Check IHDR chunk: length=13, type="IHDR", verify fields 
    ASSERT_TRUE(ReadBytes(in, buffer, 4 + 4 + 13 + 4));  // length + type + data + CRC
    EXPECT_EQ(ReadBE32(buffer.data()), 13u);
    EXPECT_EQ(std::string(reinterpret_cast<char*>(buffer.data() + 4), 4), "IHDR");
    const uint8_t* ihdr = buffer.data() + 8;
    EXPECT_EQ(ReadBE32(ihdr), width);
    EXPECT_EQ(ReadBE32(ihdr + 4), height);
    EXPECT_EQ(ihdr[8], 8);  // bit depth = 8
    EXPECT_EQ(ihdr[9], 2);  // color type = 2 (Truecolor)

    // Check IDAT chunk header: length matches compressed size, type="IDAT" 
    ASSERT_TRUE(ReadBytes(in, buffer, 4));
    EXPECT_EQ(ReadBE32(buffer.data()), compressed.size());
    ASSERT_TRUE(ReadBytes(in, buffer, 4));
    EXPECT_EQ(std::string(reinterpret_cast<char*>(buffer.data()), 4), "IDAT");
    // skip IDAT data + CRC
    in.seekg(compressed.size() + 4, std::ios::cur);

    // Check IEND chunk: zero-length data, type="IEND" 
    ASSERT_TRUE(ReadBytes(in, buffer, 12));
    EXPECT_EQ(ReadBE32(buffer.data()), 0u);
    EXPECT_EQ(std::string(reinterpret_cast<char*>(buffer.data() + 4), 4), "IEND");

    in.close();
    std::remove(file_name.c_str());
}

// Verify that WritePNG throws std::runtime_error
// when attempting to write to an invalid or non-writable path
TEST(PNGWriterTest, ThrowsOnBadPath) {
    uint64_t width = 1;
    uint64_t height = 1;
    PNGWriter writer;
    EXPECT_THROW(writer.WritePNG("/non/existent/dir/out.png", width, height, {}),
                 std::runtime_error);
}