#include "../include/image_loader.h"
#include <fstream>
#include <stdexcept>

RawImage ImageLoader::LoadRawImage(const std::string& path, uint64_t width, uint64_t height) {
    RawImage image;
    image.width = width;
    image.height = height;
    image.data.resize(width * height * 3);  // HxWx3

    std::ifstream file(path, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot open raw image file!");
    }

    file.read(reinterpret_cast<char*>(image.data.data()), image.data.size());

    if (file.gcount() != static_cast<std::streamsize>(image.data.size())) {
        throw std::runtime_error("Invalid file data! It must consist of HxWx3 bytes!");
    }

    return image;
}