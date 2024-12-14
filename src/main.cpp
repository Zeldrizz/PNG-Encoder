#include "../include/image_loader.h"
#include "../include/filter.h"
#include "../include/deflate.h"
#include "../include/png_writer.h"

#include <cstdint>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: png_encoder input.raw output.png width height [optional] filter"
                  << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];
    uint64_t width = std::stoi(argv[3]);
    uint64_t height = std::stoi(argv[4]);
    std::string filter_name = "None";

    if (argc > 5) {
        filter_name = argv[5];
    }

    try {
        RawImage image = ImageLoader::LoadRawImage(input_file, width, height);

        auto filtered_data = PNGFilter::Apply(image.data, image.width, image.height);

        auto compressed_data = DeflateCompressor::Compress(filtered_data);

        PNGWriter writer;
        writer.WritePNG(output_file, width, height, compressed_data);

        std::cout << "PNG file saved as " << output_file << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}