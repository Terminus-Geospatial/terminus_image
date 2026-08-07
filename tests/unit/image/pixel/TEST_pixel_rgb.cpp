/**
 * @file    TEST_Pixel_RGB.hpp
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/Pixel_RGB.hpp>

namespace tx = tmns::image;

/********************************************/
/*      Test Pixel RGB Math Operators       */
/********************************************/
TEST( Pixel_RGB, math_operators )
{
    // Create a pile of pixel values to use
    tx::PixelRGB_u8 p01(  0,  0,  0 );
    tx::PixelRGB_u8 p02( 10, 10, 10 );
    tx::PixelRGB_u8 p03(  1,  5, 10 );

    // Test 1:  Negation of a pixel
    {
        auto result = -p03;
        ASSERT_EQ( result[0], (uint8_t) -1 );
        ASSERT_EQ( result[1], (uint8_t) -5 );
        ASSERT_EQ( result[2], (uint8_t)-10 );
    }

    // Test 2:  Addition of 2 pixels
    {
        auto result = p02 + p03;
        ASSERT_EQ( result[0], 11 );
        ASSERT_EQ( result[1], 15 );
        ASSERT_EQ( result[2], 20 );
    }

}