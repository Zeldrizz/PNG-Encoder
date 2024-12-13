include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(png_encoder_tests
    test_image_loader.cpp
    test_filter.cpp
    test_png_writer.cpp
)

target_include_directories(png_encoder_tests 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(png_encoder_tests 
    PRIVATE 
        gtest_main
        png_encoder_lib
        ZLIB::ZLIB
)

include(GoogleTest)
gtest_discover_tests(png_encoder_tests)