/**
 * @file    TEST_channel_cast_utilities.cpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/

// C++ Libraries
#include <cstdint>

// Third-Party Libraries
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/channel_cast_utilities.hpp>
#include <terminus/image/pixel/pixel_gray.hpp>
#include <terminus/image/pixel/pixel_rgb.hpp>

namespace tx = tmns::image;

/*************************************************/
/*          Test plain channel casting           */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast )
{
    // uint16 -> uint8 (truncate)
    auto gray_u8 = tx::pix::channel_cast<uint8_t>( tx::PixelGray_u16( 100 ) );
    ASSERT_EQ( gray_u8[0], 100 );

    // uint16 RGB -> uint8 RGB (truncate low-order bits)
    auto rgb_u8 = tx::pix::channel_cast<uint8_t>( tx::PixelRGB_u16( 100, 150, 200 ) );
    ASSERT_EQ( rgb_u8[0], 100 );
    ASSERT_EQ( rgb_u8[1], 150 );
    ASSERT_EQ( rgb_u8[2], 200 );

    // float -> uint8 (plain cast truncates)
    auto f_to_u8 = tx::pix::channel_cast<uint8_t>( tx::PixelGray_f32( 0.5f ) );
    ASSERT_EQ( f_to_u8[0], 0 );

    // uint8 -> uint16 (widening)
    auto u8_to_u16 = tx::pix::channel_cast<uint16_t>( tx::PixelGray_u8( 100 ) );
    ASSERT_EQ( u8_to_u16[0], 100 );
}

/*************************************************/
/*        Test channel casting w/ rescale        */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast_rescale )
{
    // uint16 max -> uint8 max
    auto gray = tx::pix::channel_cast_rescale<uint8_t>( tx::PixelGray_u16( 65535 ) );
    ASSERT_EQ( gray[0], 255 );

    // uint8 max -> uint16 max
    auto gray_u16 = tx::pix::channel_cast_rescale<uint16_t>( tx::PixelGray_u8( 255 ) );
    ASSERT_EQ( gray_u16[0], 65535 );

    // RGB: values rescale across the full dynamic range
    auto rgb_u16 = tx::pix::channel_cast_rescale<uint16_t>( tx::PixelRGB_u8( 0, 128, 255 ) );
    ASSERT_EQ( rgb_u16[0], 0 );
    ASSERT_EQ( rgb_u16[1], 128 * 257 );        // 128 * 65535 / 255
    ASSERT_EQ( rgb_u16[2], 65535 );
}

/*************************************************/
/*          Test clamped channel cast            */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast_clamp )
{
    // Above range is clamped to max
    auto high = tx::pix::channel_cast_clamp<uint8_t>( tx::PixelGray_f32( 300.0f ) );
    ASSERT_EQ( high[0], 255 );

    // Below range is clamped to min
    auto low = tx::pix::channel_cast_clamp<uint8_t>( tx::PixelGray_f32( -10.0f ) );
    ASSERT_EQ( low[0], 0 );

    // In-range passes through
    auto mid = tx::pix::channel_cast_clamp<uint8_t>( tx::PixelGray_f32( 128.0f ) );
    ASSERT_EQ( mid[0], 128 );

    // RGB
    auto rgb = tx::pix::channel_cast_clamp<uint8_t>( tx::PixelRGB_f32( 0.0f, 128.0f, 300.0f ) );
    ASSERT_EQ( rgb[0], 0 );
    ASSERT_EQ( rgb[1], 128 );
    ASSERT_EQ( rgb[2], 255 );
}

/*************************************************/
/*          Test round channel cast              */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast_round )
{
    // Round down
    auto down = tx::pix::channel_cast_round<uint8_t>( tx::PixelGray_f32( 128.2f ) );
    ASSERT_EQ( down[0], 128 );

    // Round up
    auto up = tx::pix::channel_cast_round<uint8_t>( tx::PixelGray_f32( 128.7f ) );
    ASSERT_EQ( up[0], 129 );

    // Round .5 away from zero
    auto half = tx::pix::channel_cast_round<uint8_t>( tx::PixelGray_f32( 128.5f ) );
    ASSERT_EQ( half[0], 129 );

    // RGB
    auto rgb = tx::pix::channel_cast_round<uint8_t>( tx::PixelRGB_f32( 0.2f, 128.7f, 128.2f ) );
    ASSERT_EQ( rgb[0], 0 );
    ASSERT_EQ( rgb[1], 129 );
    ASSERT_EQ( rgb[2], 128 );
}

/*************************************************/
/*    Test round and clamped channel cast        */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast_round_and_clamp )
{
    // Above range clamped then rounded
    auto high = tx::pix::channel_cast_round_and_clamp<uint8_t>( tx::PixelGray_f32( 300.7f ) );
    ASSERT_EQ( high[0], 255 );

    // Below range clamped then rounded
    auto low = tx::pix::channel_cast_round_and_clamp<uint8_t>( tx::PixelGray_f32( -5.7f ) );
    ASSERT_EQ( low[0], 0 );

    // Normal round and clamp
    auto mid = tx::pix::channel_cast_round_and_clamp<uint8_t>( tx::PixelGray_f32( 128.5f ) );
    ASSERT_EQ( mid[0], 129 );

    // RGB
    auto rgb = tx::pix::channel_cast_round_and_clamp<uint8_t>( tx::PixelRGB_f32( 0.2f, 128.7f, 300.5f ) );
    ASSERT_EQ( rgb[0], 0 );
    ASSERT_EQ( rgb[1], 129 );
    ASSERT_EQ( rgb[2], 255 );
}

/*************************************************/
/*       Test conditional clamped cast           */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast_clamp_if_int )
{
    // Integer destination -> clamp
    auto clamped = tx::pix::channel_cast_clamp_if_int<uint8_t>( tx::PixelGray_f32( 300.0f ) );
    ASSERT_EQ( clamped[0], 255 );

    // Floating-point destination -> no clamp, plain cast
    auto unclamped = tx::pix::channel_cast_clamp_if_int<float>( tx::PixelGray_f32( 300.0f ) );
    ASSERT_FLOAT_EQ( unclamped[0], 300.0f );
}

/*************************************************/
/*       Test conditional round cast             */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast_round_if_int )
{
    // Integer destination -> round
    auto rounded = tx::pix::channel_cast_round_if_int<uint8_t>( tx::PixelGray_f32( 128.7f ) );
    ASSERT_EQ( rounded[0], 129 );

    // Floating-point destination -> no rounding
    auto unrounded = tx::pix::channel_cast_round_if_int<float>( tx::PixelGray_f32( 128.7f ) );
    ASSERT_FLOAT_EQ( unrounded[0], 128.7f );
}

/*************************************************/
/*     Test conditional round and clamp cast     */
/*************************************************/
TEST( Channel_Cast_Utilities, channel_cast_round_and_clamp_if_int )
{
    // Integer destination -> round and clamp
    auto clamp_round = tx::pix::channel_cast_round_and_clamp_if_int<uint8_t>( tx::PixelGray_f32( 300.7f ) );
    ASSERT_EQ( clamp_round[0], 255 );

    // Floating-point destination -> plain cast
    auto plain = tx::pix::channel_cast_round_and_clamp_if_int<float>( tx::PixelGray_f32( 300.7f ) );
    ASSERT_FLOAT_EQ( plain[0], 300.7f );
}
