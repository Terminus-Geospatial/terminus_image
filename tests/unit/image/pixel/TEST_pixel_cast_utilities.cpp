/**
 * @file    TEST_Pixel_Cast_Utilities.cpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#include <gtest/gtest.h>

// Terminus Image Libraries
#include <terminus/image/pixel/Pixel_Cast_Utilities.hpp>
#include <terminus/image/pixel/Pixel_Gray.hpp>

namespace tx = tmns::image;

/************************************************/
/*          Test Pixel Casting methods          */
/************************************************/
TEST( Pixel_Cast_Utilities, pixel_cast_simple )
{
    auto out_px_01 = tx::pix::pixel_cast<tx::PixelGray_u8>( tx::PixelGray_u16( 100 ) );
    ASSERT_EQ( out_px_01[0], 100 );
}