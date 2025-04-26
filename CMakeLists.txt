cmake_minimum_required(VERSION 3.14)

project(PNG_Encoder VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(ZLIB REQUIRED)

add_library(png_encoder_lib
    src/image_loader.cpp
    src/filter.cpp
    src/color_filter.cpp
    src/negative_filter.cpp
    src/grayscale_filter.cpp
    src/perlin_noise_filter.cpp
    src/deflate.cpp
    src/png_writer.cpp
)

target_include_directories(png_encoder_lib 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(png_encoder_lib PUBLIC ZLIB::ZLIB)

target_compile_options(png_encoder_lib 
    PUBLIC 
        -Wall 
        -Wextra 
        -Werror
)

add_executable(png_encoder
    src/main.cpp
)

target_link_libraries(png_encoder PUBLIC png_encoder_lib)

enable_testing()

add_subdirectory(tests)