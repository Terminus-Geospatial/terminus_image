/**
 * @file    TEST_read_image_disk.cpp
 * @author  Marvin Smith
 * @date    7/28/2023
 */
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/log/utility.hpp>
#include <terminus/image/io/read_image_disk.hpp>
#include <terminus/image/io/write_image.hpp>
#include <terminus/image/operations/normalize.hpp>
#include <terminus/image/operations/statistics/pixel_operations.hpp>
#include <terminus/image/pixel/pixel_rgb.hpp>
#include <terminus/image/types/image_memory.hpp>
#include <terminus/image/utility/view_utilities.hpp>

// Test Libraries
#include "../../UNIT_TEST_ONLY/Test_Environment.hpp"

/***************************************************/
/*          Read and write imagery (JPEG)          */
/***************************************************/
TEST( io_read_image_disk, read_disk_jpg )
{
    namespace tx = tmns::image;

    // Load an image
    std::filesystem::path image_to_load { "./data/images/jpeg/lena.jpg" };
    tmns::log::trace( ADD_CURRENT_LOC(), "Loading Image" );
    auto result = tx::io::read_image_disk<tx::PixelRGB_u8>( image_to_load );
    tmns::log::trace( ADD_CURRENT_LOC(), "End of read_image_disk" );

    if( result.has_error() )
    {
        tmns::log::error( ADD_CURRENT_LOC(),
                          "error reading image: ",
                          result.error().message() );
    }
    ASSERT_FALSE( result.has_error() );
    auto image = result.assume_value();

    ASSERT_FALSE( result.has_error() );

    // View the image
    if( Test_Environment::use_gui() )
    {
        auto res = tx::utility::view_image( "Dummy Window", image );
    }

    ASSERT_EQ( image.cols(), 512 );
    ASSERT_EQ( image.rows(), 512 );
    ASSERT_EQ( image.channels(), 3 );
    ASSERT_EQ( image.format().channel_type(), tx::Channel_Type_Enum::UINT8 );
    ASSERT_EQ( image.format().pixel_type(),   tx::Pixel_Format_Enum::RGB );

    // Compute the mean of the image
    auto mean_pixel_value = tmns::image::ops::mean_pixel_value( image );

    ASSERT_NEAR( mean_pixel_value[0], 180.214, 0.1 );
    ASSERT_NEAR( mean_pixel_value[1],  99.049, 0.1 );
    ASSERT_NEAR( mean_pixel_value[2], 105.415, 0.1 );
}

/***************************************************/
/*          Read and write imagery (ISIS)          */
/***************************************************/
TEST( io_read_image_disk, read_disk_isis_cube )
{
    namespace tx = tmns::image;

    // Load an image
    const std::string IMAGE_KEY = "NASA_HIRISE_PDS_IMAGE_01";
    auto image_to_load = Test_Environment::get_image_datastore().get_path( IMAGE_KEY );
    if( !image_to_load )
    {
        tmns::log::warn( "Skipping test as key not found: ", IMAGE_KEY );
        GTEST_SKIP();
    }
    if( !std::filesystem::exists( *image_to_load ) )
    {
        tmns::log::warn( "Skipping test as input image not found: ", *image_to_load );
        GTEST_SKIP();
    }
    tmns::log::trace( ADD_CURRENT_LOC(), "Loading Image: ", *image_to_load );
    auto result = tx::io::read_image_disk<tx::PixelGray_f32>( *image_to_load );
    tmns::log::trace( ADD_CURRENT_LOC(), "End of read_image_disk" );

    if( result.has_error() )
    {
        tmns::log::error( ADD_CURRENT_LOC(), "error reading image: ", result.error().message() );
    }
    ASSERT_FALSE( result.has_error() );
    auto image = result.assume_value();

    // Print metadata
    tmns::log::debug( "Image Metadata:\n", image.metadata()->to_log_string() );


    ASSERT_FALSE( result.has_error() );

    // View the image
    if( false )
    {
        auto res = tx::utility::view_image( "Dummy Window", image );
    }

    ASSERT_EQ( image.cols(), 1024 );
    ASSERT_EQ( image.rows(), 25000 );
    ASSERT_EQ( image.channels(), 1 );
    ASSERT_EQ( image.format().channel_type(), tx::Channel_Type_Enum::FLOAT32 );
    ASSERT_EQ( image.format().pixel_type(),   tx::Pixel_Format_Enum::GRAY );

    // Compute the mean of the image
    auto mean_pixel_value = tmns::image::ops::mean_pixel_value( image );

    ASSERT_NEAR( mean_pixel_value[0], 0.082, 0.1 );
}