/**
 * @file    TEST_channel_premultiply.cpp
 * @author  Marvin Smith
 * @date    8/3/2026
 *
 * @brief   Unit tests for channel_premultiply_float, channel_unpremultiply_float,
 *          channel_premultiply_int, channel_unpremultiply_int, and channel_average,
 *          exercised through the public convert() API.
*/

// C++ Standard Libraries
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

// GoogleTest
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/convert.hpp>
#include <terminus/image/pixel/pixel_format_enum.hpp>
#include <terminus/image/types/image_buffer.hpp>
#include <terminus/image/types/image_format.hpp>

namespace tx = tmns::image;

/**
 * Helper: Build an Image_Buffer around an existing pixel array.
 *
 * @param data        Pointer to raw pixel data
 * @param cols        Number of columns
 * @param rows        Number of rows
 * @param planes      Number of planes
 * @param pixel_type  Pixel format enum
 * @param channel_type Channel type enum
 * @param premultiply Premultiply flag
 * @param cstride     Column stride in bytes
 * @param rstride     Row stride in bytes
 * @param pstride     Plane stride in bytes
 */
static tx::Image_Buffer make_buffer( void*                 data,
                                     int                   cols,
                                     int                   rows,
                                     int                   planes,
                                     tx::Pixel_Format_Enum pixel_type,
                                     tx::Channel_Type_Enum channel_type,
                                     bool                  premultiply,
                                     size_t                cstride,
                                     size_t                rstride,
                                     size_t                pstride )
{
    tx::Image_Format fmt( cols, rows, planes,
                          pixel_type, channel_type,
                          premultiply );

    return tx::Image_Buffer( data, fmt, cstride, rstride, pstride );
}

// ============================================================================
// Float premultiply / unpremultiply round-trip
// ============================================================================

/**
 * Verify that converting RGBA-float data from premultiplied -> non-premultiplied
 * and back produces the original values within tolerance.
 */
TEST( channel_premultiply, float_rgba_round_trip )
{
    // RGBA pixel: R=0.6, G=0.3, B=0.9, A=0.5
    // Premultiplied form: R=0.3, G=0.15, B=0.45, A=0.5
    std::array<float, 4> premul_src = { 0.3f, 0.15f, 0.45f, 0.5f };
    std::array<float, 4> straight_dst = { 0, 0, 0, 0 };

    const size_t ch_size  = sizeof(float);
    const size_t px_size  = 4 * ch_size;

    // Source: premultiplied RGBA float
    auto src_buf = make_buffer( premul_src.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                true,   // premultiplied
                                px_size, px_size, px_size );

    // Destination: straight (non-premultiplied) RGBA float
    auto dst_buf = make_buffer( straight_dst.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                false,  // not premultiplied
                                px_size, px_size, px_size );

    // Convert premultiplied -> straight
    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // Check straight values: R=0.6, G=0.3, B=0.9, A=0.5
    EXPECT_NEAR( straight_dst[0], 0.6f,  1e-5f );
    EXPECT_NEAR( straight_dst[1], 0.3f,  1e-5f );
    EXPECT_NEAR( straight_dst[2], 0.9f,  1e-5f );
    EXPECT_NEAR( straight_dst[3], 0.5f,  1e-5f );

    // Now convert straight -> premultiplied (round-trip)
    std::array<float, 4> premul_dst = { 0, 0, 0, 0 };
    auto premul_dst_buf = make_buffer( premul_dst.data(), 1, 1, 1,
                                       tx::Pixel_Format_Enum::RGBA,
                                       tx::Channel_Type_Enum::FLOAT32,
                                       true,  // premultiplied
                                       px_size, px_size, px_size );

    auto straight_src_buf = make_buffer( straight_dst.data(), 1, 1, 1,
                                         tx::Pixel_Format_Enum::RGBA,
                                         tx::Channel_Type_Enum::FLOAT32,
                                         false,
                                         px_size, px_size, px_size );

    result = tx::convert( premul_dst_buf, straight_src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // Should match original premultiplied values
    EXPECT_NEAR( premul_dst[0], premul_src[0], 1e-5f );
    EXPECT_NEAR( premul_dst[1], premul_src[1], 1e-5f );
    EXPECT_NEAR( premul_dst[2], premul_src[2], 1e-5f );
    EXPECT_NEAR( premul_dst[3], premul_src[3], 1e-5f );
}

// ============================================================================
// Float premultiply with full opacity (alpha = 1.0)
// ============================================================================

/**
 * When alpha is 1.0, premultiplied and straight should be identical.
 */
TEST( channel_premultiply, float_full_opacity )
{
    std::array<float, 4> src_pixels = { 0.25f, 0.50f, 0.75f, 1.0f };
    std::array<float, 4> dst_pixels = { 0, 0, 0, 0 };

    const size_t ch_size = sizeof(float);
    const size_t px_size = 4 * ch_size;

    // Straight -> premultiplied with alpha=1.0
    auto src_buf = make_buffer( src_pixels.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                false,
                                px_size, px_size, px_size );

    auto dst_buf = make_buffer( dst_pixels.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                true,
                                px_size, px_size, px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // With alpha=1.0, premultiplied values should equal straight values
    EXPECT_NEAR( dst_pixels[0], 0.25f, 1e-5f );
    EXPECT_NEAR( dst_pixels[1], 0.50f, 1e-5f );
    EXPECT_NEAR( dst_pixels[2], 0.75f, 1e-5f );
    EXPECT_NEAR( dst_pixels[3], 1.0f,  1e-5f );
}

// ============================================================================
// Float premultiply with zero alpha
// ============================================================================

/**
 * When alpha is 0.0, premultiplied RGB channels should be 0.
 */
TEST( channel_premultiply, float_zero_alpha_premultiply )
{
    std::array<float, 4> src_pixels = { 0.5f, 0.5f, 0.5f, 0.0f };
    std::array<float, 4> dst_pixels = { 9, 9, 9, 9 };

    const size_t ch_size = sizeof(float);
    const size_t px_size = 4 * ch_size;

    // Straight -> premultiplied with alpha=0
    auto src_buf = make_buffer( src_pixels.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                false,
                                px_size, px_size, px_size );

    auto dst_buf = make_buffer( dst_pixels.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                true,
                                px_size, px_size, px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // With alpha=0, all premultiplied channels should be zero
    EXPECT_NEAR( dst_pixels[0], 0.0f, 1e-5f );
    EXPECT_NEAR( dst_pixels[1], 0.0f, 1e-5f );
    EXPECT_NEAR( dst_pixels[2], 0.0f, 1e-5f );
    EXPECT_NEAR( dst_pixels[3], 0.0f, 1e-5f );
}

// ============================================================================
// Double premultiply / unpremultiply round-trip
// ============================================================================

/**
 * Same round-trip test but with double precision to verify
 * both float and double template instantiations.
 */
TEST( channel_premultiply, double_rgba_round_trip )
{
    std::array<double, 4> premul_src = { 0.3, 0.15, 0.45, 0.5 };
    std::array<double, 4> straight_dst = { 0, 0, 0, 0 };

    const size_t ch_size = sizeof(double);
    const size_t px_size = 4 * ch_size;

    auto src_buf = make_buffer( premul_src.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT64,
                                true,
                                px_size, px_size, px_size );

    auto dst_buf = make_buffer( straight_dst.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT64,
                                false,
                                px_size, px_size, px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    EXPECT_NEAR( straight_dst[0], 0.6,  1e-12 );
    EXPECT_NEAR( straight_dst[1], 0.3,  1e-12 );
    EXPECT_NEAR( straight_dst[2], 0.9,  1e-12 );
    EXPECT_NEAR( straight_dst[3], 0.5,  1e-12 );

    // Round-trip back
    std::array<double, 4> premul_dst = { 0, 0, 0, 0 };
    auto premul_dst_buf = make_buffer( premul_dst.data(), 1, 1, 1,
                                       tx::Pixel_Format_Enum::RGBA,
                                       tx::Channel_Type_Enum::FLOAT64,
                                       true,
                                       px_size, px_size, px_size );

    auto straight_src_buf = make_buffer( straight_dst.data(), 1, 1, 1,
                                         tx::Pixel_Format_Enum::RGBA,
                                         tx::Channel_Type_Enum::FLOAT64,
                                         false,
                                         px_size, px_size, px_size );

    result = tx::convert( premul_dst_buf, straight_src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    EXPECT_NEAR( premul_dst[0], premul_src[0], 1e-12 );
    EXPECT_NEAR( premul_dst[1], premul_src[1], 1e-12 );
    EXPECT_NEAR( premul_dst[2], premul_src[2], 1e-12 );
    EXPECT_NEAR( premul_dst[3], premul_src[3], 1e-12 );
}

// ============================================================================
// GrayA (2-channel) float premultiply round-trip
// ============================================================================

/**
 * Test with GRAYA (2-channel) to exercise the premultiply path
 * with a shorter channel count.
 */
TEST( channel_premultiply, float_graya_round_trip )
{
    // GrayA: Gray=0.8, A=0.5 -> premultiplied Gray=0.4, A=0.5
    std::array<float, 2> premul_src = { 0.4f, 0.5f };
    std::array<float, 2> straight_dst = { 0, 0 };

    const size_t ch_size = sizeof(float);
    const size_t px_size = 2 * ch_size;

    auto src_buf = make_buffer( premul_src.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::GRAYA,
                                tx::Channel_Type_Enum::FLOAT32,
                                true,
                                px_size, px_size, px_size );

    auto dst_buf = make_buffer( straight_dst.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::GRAYA,
                                tx::Channel_Type_Enum::FLOAT32,
                                false,
                                px_size, px_size, px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    EXPECT_NEAR( straight_dst[0], 0.8f, 1e-5f );
    EXPECT_NEAR( straight_dst[1], 0.5f, 1e-5f );

    // Round-trip back
    std::array<float, 2> premul_dst = { 0, 0 };
    auto premul_dst_buf = make_buffer( premul_dst.data(), 1, 1, 1,
                                       tx::Pixel_Format_Enum::GRAYA,
                                       tx::Channel_Type_Enum::FLOAT32,
                                       true,
                                       px_size, px_size, px_size );

    auto straight_src_buf = make_buffer( straight_dst.data(), 1, 1, 1,
                                         tx::Pixel_Format_Enum::GRAYA,
                                         tx::Channel_Type_Enum::FLOAT32,
                                         false,
                                         px_size, px_size, px_size );

    result = tx::convert( premul_dst_buf, straight_src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    EXPECT_NEAR( premul_dst[0], premul_src[0], 1e-5f );
    EXPECT_NEAR( premul_dst[1], premul_src[1], 1e-5f );
}

// ============================================================================
// Integer (uint8) premultiply / unpremultiply round-trip
// ============================================================================

/**
 * Test the integer premultiply path with uint8 RGBA data.
 * Integer premultiply uses:  scale = alpha / 255
 * So R_premul = round( R_straight * scale )
 */
TEST( channel_premultiply, uint8_rgba_round_trip )
{
    // Straight RGBA: R=200, G=100, B=50, A=128 (~0.502 alpha)
    // Premultiplied: R~100, G~50, B~25, A=128
    std::array<uint8_t, 4> straight_src = { 200, 100, 50, 128 };
    std::array<uint8_t, 4> premul_dst   = { 0, 0, 0, 0 };

    const size_t ch_size = sizeof(uint8_t);
    const size_t px_size = 4 * ch_size;

    auto src_buf = make_buffer( straight_src.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::UINT8,
                                false,
                                px_size, px_size, px_size );

    auto dst_buf = make_buffer( premul_dst.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::UINT8,
                                true,
                                px_size, px_size, px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // Alpha should be preserved exactly
    EXPECT_EQ( premul_dst[3], 128 );

    // Premultiplied channels should be roughly src * (128/255)
    // Allow +-1 for rounding
    EXPECT_NEAR( premul_dst[0], 100, 2 );
    EXPECT_NEAR( premul_dst[1],  50, 2 );
    EXPECT_NEAR( premul_dst[2],  25, 2 );

    // Now unpremultiply back
    std::array<uint8_t, 4> straight_dst = { 0, 0, 0, 0 };
    auto premul_src_buf = make_buffer( premul_dst.data(), 1, 1, 1,
                                       tx::Pixel_Format_Enum::RGBA,
                                       tx::Channel_Type_Enum::UINT8,
                                       true,
                                       px_size, px_size, px_size );

    auto straight_dst_buf = make_buffer( straight_dst.data(), 1, 1, 1,
                                         tx::Pixel_Format_Enum::RGBA,
                                         tx::Channel_Type_Enum::UINT8,
                                         false,
                                         px_size, px_size, px_size );

    result = tx::convert( straight_dst_buf, premul_src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // Round-trip should be close to original (within +-2 due to integer rounding)
    EXPECT_NEAR( straight_dst[0], straight_src[0], 2 );
    EXPECT_NEAR( straight_dst[1], straight_src[1], 2 );
    EXPECT_NEAR( straight_dst[2], straight_src[2], 2 );
    EXPECT_EQ(   straight_dst[3], straight_src[3] );
}

// ============================================================================
// Integer (uint8) premultiply with full opacity
// ============================================================================

TEST( channel_premultiply, uint8_full_opacity )
{
    std::array<uint8_t, 4> src_pixels = { 100, 150, 200, 255 };
    std::array<uint8_t, 4> dst_pixels = { 0, 0, 0, 0 };

    const size_t ch_size = sizeof(uint8_t);
    const size_t px_size = 4 * ch_size;

    auto src_buf = make_buffer( src_pixels.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::UINT8,
                                false,
                                px_size, px_size, px_size );

    auto dst_buf = make_buffer( dst_pixels.data(), 1, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::UINT8,
                                true,
                                px_size, px_size, px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // With alpha=255, premultiplied == straight for uint8
    EXPECT_EQ( dst_pixels[0], 100 );
    EXPECT_EQ( dst_pixels[1], 150 );
    EXPECT_EQ( dst_pixels[2], 200 );
    EXPECT_EQ( dst_pixels[3], 255 );
}

// ============================================================================
// Multi-pixel float premultiply (2x1 image)
// ============================================================================

/**
 * Verify the premultiply path works across a multi-pixel buffer.
 */
TEST( channel_premultiply, float_multi_pixel )
{
    // 2 RGBA pixels, straight alpha
    // Pixel 0: R=1.0, G=0.5, B=0.0, A=0.5
    // Pixel 1: R=0.0, G=1.0, B=0.5, A=0.25
    std::array<float, 8> src_pixels = { 1.0f, 0.5f, 0.0f, 0.5f,
                                        0.0f, 1.0f, 0.5f, 0.25f };
    std::array<float, 8> dst_pixels = {};

    const size_t ch_size = sizeof(float);
    const size_t px_size = 4 * ch_size;

    auto src_buf = make_buffer( src_pixels.data(), 2, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                false,
                                px_size,       // cstride
                                2 * px_size,   // rstride
                                2 * px_size ); // pstride

    auto dst_buf = make_buffer( dst_pixels.data(), 2, 1, 1,
                                tx::Pixel_Format_Enum::RGBA,
                                tx::Channel_Type_Enum::FLOAT32,
                                true,
                                px_size,
                                2 * px_size,
                                2 * px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    // Pixel 0: premul = straight * 0.5
    EXPECT_NEAR( dst_pixels[0], 0.50f,  1e-5f );
    EXPECT_NEAR( dst_pixels[1], 0.25f,  1e-5f );
    EXPECT_NEAR( dst_pixels[2], 0.00f,  1e-5f );
    EXPECT_NEAR( dst_pixels[3], 0.50f,  1e-5f );

    // Pixel 1: premul = straight * 0.25
    EXPECT_NEAR( dst_pixels[4], 0.000f, 1e-5f );
    EXPECT_NEAR( dst_pixels[5], 0.250f, 1e-5f );
    EXPECT_NEAR( dst_pixels[6], 0.125f, 1e-5f );
    EXPECT_NEAR( dst_pixels[7], 0.250f, 1e-5f );
}

// ============================================================================
// Channel average (RGB -> GRAY conversion)
// ============================================================================

/**
 * Converting RGB float to GRAY float should average the 3 channels,
 * exercising channel_average.
 */
TEST( channel_average, float_rgb_to_gray )
{
    // RGB: R=0.3, G=0.6, B=0.9  -> Average = 0.6
    std::array<float, 3> src_pixels = { 0.3f, 0.6f, 0.9f };
    std::array<float, 1> dst_pixels = { 0 };

    const size_t src_ch_size = sizeof(float);
    const size_t src_px_size = 3 * src_ch_size;
    const size_t dst_px_size = 1 * src_ch_size;

    tx::Image_Format src_fmt( 1, 1, 1,
                              tx::Pixel_Format_Enum::RGB,
                              tx::Channel_Type_Enum::FLOAT32,
                              true );
    tx::Image_Buffer src_buf( src_pixels.data(), src_fmt,
                              src_px_size, src_px_size, src_px_size );

    tx::Image_Format dst_fmt( 1, 1, 1,
                              tx::Pixel_Format_Enum::GRAY,
                              tx::Channel_Type_Enum::FLOAT32,
                              true );
    tx::Image_Buffer dst_buf( dst_pixels.data(), dst_fmt,
                              dst_px_size, dst_px_size, dst_px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    EXPECT_NEAR( dst_pixels[0], 0.6f, 1e-5f );
}

/**
 * Channel average with uint8 data (RGB -> GRAY).
 */
TEST( channel_average, uint8_rgb_to_gray )
{
    // RGB: R=60, G=120, B=180 -> Average = 120
    std::array<uint8_t, 3> src_pixels = { 60, 120, 180 };
    std::array<uint8_t, 1> dst_pixels = { 0 };

    const size_t src_px_size = 3 * sizeof(uint8_t);
    const size_t dst_px_size = 1 * sizeof(uint8_t);

    tx::Image_Format src_fmt( 1, 1, 1,
                              tx::Pixel_Format_Enum::RGB,
                              tx::Channel_Type_Enum::UINT8,
                              true );
    tx::Image_Buffer src_buf( src_pixels.data(), src_fmt,
                              src_px_size, src_px_size, src_px_size );

    tx::Image_Format dst_fmt( 1, 1, 1,
                              tx::Pixel_Format_Enum::GRAY,
                              tx::Channel_Type_Enum::UINT8,
                              true );
    tx::Image_Buffer dst_buf( dst_pixels.data(), dst_fmt,
                              dst_px_size, dst_px_size, dst_px_size );

    auto result = tx::convert( dst_buf, src_buf );
    ASSERT_FALSE( result.has_error() ) << result.error().details();

    EXPECT_EQ( dst_pixels[0], 120 );
}
