#include <cstdint>
#include <iostream>
#include "../include/image_loader.h"

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: png_encoder input.raw output.png width height" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];
    uint64_t width = std::stoi(argv[3]);
    uint64_t height = std::stoi(argv[4]);

    try {
        RawImage image = ImageLoader::LoadRawImage(input_file, width, height);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}