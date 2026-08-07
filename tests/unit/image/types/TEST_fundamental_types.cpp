/**
 * @file    TEST_fundamental_types.cpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/pixel_rgba.hpp>
#include <terminus/math/types/fundamental_types.hpp>

/******************************************/
/*      Test the Is-Scalar Type-Trait     */
/******************************************/
TEST( Fundamental_Types, Is_Scalar )
{
    ASSERT_FALSE( tmns::math::Is_Scalar<tmns::image::PixelRGBA_u8>::value );
}
