/**
 * @file    TEST_read_image.cpp
 * @author  Marvin Smith
 * @date    7/7/2023
 */
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/log/utility.hpp>
#include <terminus/image/io/read_image.hpp>
#include <terminus/image/pixel/channel_types.hpp>
#include <terminus/image/pixel/pixel_rgb.hpp>
#include <terminus/image/types/image_memory.hpp>
#include <terminus/image/utility/view_utilities.hpp>

/********************************************/
/*          Read Image From Disk            */
/********************************************/
TEST( io_read_image, read_image_memory )
{
    namespace wc = tmns::image;
    FAIL();
    /*
    // Load an image
    std::filesystem::path image_to_load { "./data/images/jpeg/lena.jpg" };
    auto result = wc::io::read_image<wc::PixelRGB_u8>( image_to_load );

    if( result.has_error() )
    {
        tmns::log::error( "error reading image: ", result.error().message() );
    }
    ASSERT_FALSE( result.has_error() );
    auto image = result.assume_value();

    ASSERT_FALSE( result.has_error() );

    // View the image
    if( true )
    {
        auto res = wc::utility::view_image( "Dummy Window", image );
    }*/

    FAIL();
}