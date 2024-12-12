cmake_minimum_required(VERSION 3.10)

project(PNG_Encoding VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(ZLIB REQUIRED)

option(BUILD_TESTS "Build tests" OFF)

add_executable(png_encoder
    src/main.cpp
    src/image_loader.cpp
    src/filter.cpp
    src/deflate.cpp
    src/png_writer.cpp
    include/image_loader.h
    include/filter.h
    include/deflate.h
    include/png_writer.h
)

target_include_directories(png_encoder 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(png_encoder PUBLIC ZLIB::ZLIB)

target_compile_options(png_encoder 
    PUBLIC 
        -Wall 
        -Wextra 
        -Werror
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()