/**
 * @file    TEST_Image_Memory.cpp
 * @author  Marvin Smith
 * @date    7/28/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/Pixel_RGBA.hpp>
#include <terminus/image/types/Image_Memory.hpp>

namespace tx = tmns::image;

/**********************************************/
/*      Perform a set of simple checks on     */
/*      an RGBA-u8 image.                     */
/**********************************************/
TEST( Image_Memory, basic_checks_RGBA_uint8 )
{
    // Create an image
    tx::Image_Memory<tx::PixelRGBA_u8> test_image_01( 100, 200, 1);
    ASSERT_EQ( test_image_01.cols(), 100 );
    ASSERT_EQ( test_image_01.rows(), 200 );
    ASSERT_EQ( test_image_01.planes(), 1 );

    auto buffer_01 = test_image_01.buffer();
    ASSERT_NE( buffer_01.data(), nullptr );
    ASSERT_EQ( buffer_01.cols(), 100 );
    ASSERT_EQ( buffer_01.rows(), 200 );
    ASSERT_EQ( buffer_01.planes(), 1 );
    ASSERT_EQ( buffer_01.pixel_type(), tx::Pixel_Format_Enum::RGBA );
    ASSERT_EQ( buffer_01.channel_type(), tx::Channel_Type_Enum::UINT8 );

    tmns::log::trace( buffer_01.to_string() );
}