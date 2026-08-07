/**
 * @file    TEST_pixel_cast_utilities.cpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#include <gtest/gtest.h>

// Terminus Image Libraries
#include <terminus/image/pixel/pixel_cast_utilities.hpp>
#include <terminus/image/pixel/pixel_gray.hpp>

namespace tx = tmns::image;

/************************************************/
/*          Test Pixel Casting methods          */
/************************************************/
TEST( Pixel_Cast_Utilities, pixel_cast_simple )
{
    auto out_px_01 = tx::pix::pixel_cast<tx::PixelGray_u8>( tx::PixelGray_u16( 100 ) );
    ASSERT_EQ( out_px_01[0], 100 );
}