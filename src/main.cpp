// main.cpp
#include "../include/image_loader.h"
#include "../include/filter.h"
#include "../include/color_filter.h"
#include "../include/deflate.h"
#include "../include/png_writer.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 5 && argc != 6 && argc != 7) {
        std::cerr << "Usage:\n"
                     "  png_encoder in.raw out.png W H\n"
                     "  png_encoder in.raw out.png W H <filter>\n"
                     "  png_encoder in.raw out.png W H perlin <0-100>\n";
        return 1;
    }

    const std::string input_file = argv[1];
    const std::string output_file = argv[2];
    const uint64_t image_width = std::stoull(argv[3]);
    const uint64_t image_height = std::stoull(argv[4]);

    std::string filter_option = "none";
    float perlin_strength = 0.f;

    if (argc >= 6) {
        filter_option = argv[5];
        std::transform(filter_option.begin(), filter_option.end(), filter_option.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        if (filter_option == "perlin") {
            if (argc == 7) {
                perlin_strength = std::stof(argv[6]);
            }
        } else if (argc == 7) {
            std::cerr << "Error: extra parameter after '" << filter_option << "' is not allowed.\n";
            return 1;
        }
    }

    try {
        RawImage raw_image = ImageLoader::LoadRawImage(input_file, image_width, image_height);

        ColorFilterType filter_type = ColorFilter::Parse(filter_option);

        std::vector<uint8_t> filtered_rgb_data = ColorFilter::Apply(
            raw_image.data, image_width, image_height, filter_type, perlin_strength);

        std::vector<uint8_t> scanlines =
            PNGFilter::Apply(filtered_rgb_data, image_width, image_height);

        std::vector<uint8_t> compressed_data = DeflateCompressor::Compress(scanlines);

        PNGWriter png_writer;
        png_writer.WritePNG(output_file, image_width, image_height, compressed_data);

        std::cout << "PNG file saved as " << output_file << '\n';
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}