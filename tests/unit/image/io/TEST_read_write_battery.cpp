/**
 * @file    TEST_read_write_battery.cpp
 * @author  Marvin Smith
 * @date    7/29/2023
*/
#include <gtest/gtest.h>


// Terminus Libraries
#include <terminus/image/io/read_image_disk.hpp>
#include <terminus/image/io/write_image.hpp>
#include <terminus/image/utility/view_utilities.hpp>

/*****************************************************/
/*      Test Read and Write Small RGBA JPG to PNG    */
/*****************************************************/
TEST( io_read_write_battery, small_rgba_jpg_to_png )
{
    namespace wc = tmns::image;

    // Load an image
    std::filesystem::path image_to_load { "./data/images/jpeg/lena.jpg" };
    std::filesystem::path image_to_write { "./test1.tif" };

    tmns::log::trace( ADD_CURRENT_LOC(), "Loading Image" );
    auto result = wc::io::read_image_disk<wc::PixelRGB_u8>( image_to_load );
    tmns::log::trace( ADD_CURRENT_LOC(), "End of read_image_disk" );

    if( result.has_error() )
    {
        tmns::log::error( ADD_CURRENT_LOC(), "error reading image: ", result.error().message() );
    }
    ASSERT_FALSE( result.has_error() );

    auto image = result.assume_value();
    if( result.has_error() )
    {
        tmns::log::error( ADD_CURRENT_LOC(), "about to fail, ", result.error().message() );
    }
    ASSERT_FALSE( result.has_error() );

    // Write the image
    tmns::log::trace( ADD_CURRENT_LOC(), "Calling write_image(), format:", image.format().to_string() );
    auto wt_res = tmns::image::io::write_image( image_to_write,
                                                image );
    if( wt_res.has_error() )
    {
        tmns::log::error( ADD_CURRENT_LOC(), "about to fail, ", wt_res.error().message() );
    }

    // Load this image
    auto test_res = tmns::image::io::read_image_disk<wc::PixelRGB_f64>( image_to_write );
    auto test_image = test_res.value();

    // Compare images
    ASSERT_EQ( image.rows(), test_image.rows() );
    ASSERT_EQ( image.cols(), test_image.cols() );
    ASSERT_EQ( image.channels(), test_image.channels() );
    ASSERT_EQ( image.channels(), test_image.channels() );

}