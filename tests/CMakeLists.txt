# tests/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(png_encoder_tests
    test_image_loader.cpp
    test_filter.cpp
    test_deflate.cpp
    test_png_writer.cpp
)

target_include_directories(png_encoder_tests 
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(png_encoder_tests 
    PRIVATE 
        Catch2::Catch2WithMain
        ZLIB::ZLIB
)

include(Catch)
catch_discover_tests(png_encoder_tests)