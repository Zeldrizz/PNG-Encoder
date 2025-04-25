// png_writer.h
#pragma once

#include <cstdint>
#include <vector>
#include <string>

class PNGWriter {
public:
    PNGWriter();
    void WritePNG(const std::string& filename, uint64_t width, uint64_t height,
                  const std::vector<uint8_t>& compressed_data);

private:
    static constexpr char kIHDRChunkType[5] = "IHDR";
    static constexpr char kIDATChunkType[5] = "IDAT";
    static constexpr char kIENDChunkType[5] = "IEND";

    static constexpr uint8_t kPNGSignature[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'};

private:
    void InitializeCRCTable();
    uint32_t CalculateCRC(const uint8_t* buffer, size_t length) const;
    void WriteUInt32(std::ofstream& out, uint32_t value) const;
    void WriteChunk(std::ofstream& out, const std::string& type,
                    const std::vector<uint8_t>& data) const;

    uint32_t crc_table_[256];
    bool crc_table_computed_;
};