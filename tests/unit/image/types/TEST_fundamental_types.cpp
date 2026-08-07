/**
 * @file    TEST_Fundamental_Types.cpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/Pixel_RGBA.hpp>
#include <terminus/math/types/Fundamental_Types.hpp>

/******************************************/
/*      Test the Is-Scalar Type-Trait     */
/******************************************/
TEST( Fundamental_Types, Is_Scalar )
{
    ASSERT_FALSE( tmns::math::Is_Scalar<tmns::image::PixelRGBA_u8>::value );
}
