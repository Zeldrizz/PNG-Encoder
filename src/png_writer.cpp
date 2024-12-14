#include "../include/png_writer.h"
#include <fstream>
#include <stdexcept>
#include <cstring>

PNGWriter::PNGWriter() : crc_table_computed_(false) {
    InitializeCRCTable();
}

void PNGWriter::InitializeCRCTable() {
    for (int n = 0; n < 256; ++n) {
        uint32_t c = static_cast<uint32_t>(n);

        for (int k = 0; k < 8; ++k) {
            if (c & 1) {
                c = 0xEDB88320L ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }

        crc_table_[n] = c;
    }

    crc_table_computed_ = true;
}

uint32_t PNGWriter::CalculateCRC(const uint8_t* buffer, size_t length) const {
    uint32_t crc = 0xFFFFFFFF;

    for (size_t i = 0; i < length; ++i) {
        uint8_t index = (crc ^ buffer[i]) & 0xFF;
        crc = crc_table_[index] ^ (crc >> 8);
    }

    return crc ^ 0xFFFFFFFF;
}

void PNGWriter::WriteUInt32(std::ofstream& out, uint32_t value) const {
    uint8_t bytes[4];

    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;

    out.write(reinterpret_cast<char*>(bytes), 4);
}

void PNGWriter::WriteChunk(std::ofstream& out, const std::string& type,
                           const std::vector<uint8_t>& data) const {
    WriteUInt32(out, static_cast<uint32_t>(data.size()));

    out.write(type.c_str(), 4);

    if (!data.empty()) {
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    std::vector<uint8_t> type_and_data;
    type_and_data.reserve(4 + data.size());
    type_and_data.insert(type_and_data.end(), type.begin(), type.end());
    type_and_data.insert(type_and_data.end(), data.begin(), data.end());

    uint32_t crc_val = CalculateCRC(type_and_data.data(), type_and_data.size());

    WriteUInt32(out, crc_val);
}

void PNGWriter::WritePNG(const std::string& filename, uint64_t width, uint64_t height,
                         const std::vector<uint8_t>& compressed_data) {
    std::ofstream out(filename, std::ios::binary);

    if (!out) {
        throw std::runtime_error("Error with output PNG file!");
    }

    // Записываем сигнатуру PNG
    out.write(reinterpret_cast<const char*>(kPNGSignature), sizeof(kPNGSignature));

    // Создаем и записываем чанк IHDR
    std::vector<uint8_t> ihdr(13, 0);

    // Ширина (4 байта, big-endian)
    ihdr[0] = (width >> 24) & 0xFF;
    ihdr[1] = (width >> 16) & 0xFF;
    ihdr[2] = (width >> 8) & 0xFF;
    ihdr[3] = width & 0xFF;

    // Высота (4 байта, big-endian)
    ihdr[4] = (height >> 24) & 0xFF;
    ihdr[5] = (height >> 16) & 0xFF;
    ihdr[6] = (height >> 8) & 0xFF;
    ihdr[7] = height & 0xFF;

    ihdr[8] = 8;   // Bit depth
    ihdr[9] = 2;   // Color type: Truecolor
    ihdr[10] = 0;  // Compression method
    ihdr[11] = 0;  // Filter method
    ihdr[12] = 0;  // Interlace method

    WriteChunk(out, std::string(kIHDRChunkType, 4), ihdr);

    // Создаем и записываем чанк IDAT
    WriteChunk(out, std::string(kIDATChunkType, 4), compressed_data);

    // Создаем и записываем чанк IEND
    std::vector<uint8_t> iend_data;
    WriteChunk(out, std::string(kIENDChunkType, 4), iend_data);
}