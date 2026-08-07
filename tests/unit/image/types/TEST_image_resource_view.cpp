/**
 * @file    TEST_Image_Resource_View.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/Pixel_Gray.hpp>
#include <terminus/image/pixel/Pixel_RGBA.hpp>
#include <terminus/image/types/Image_Resource_View.hpp>

namespace tx = tmns::image;

/******************************************************************/
/*      Do a check using a GDAL Disk Resource with a JPG image    */
/******************************************************************/
TEST( types_Image_Resource_View, simple_test_disk_gdal_jpg )
{
    std::filesystem::path image_to_load { "./data/images/jpeg/lena.jpg" };

    // Create image resource
    auto resource = std::make_shared<tx::io::gdal::Image_Resource_Disk_GDAL>( image_to_load );

    // Check it's status
    ASSERT_EQ( resource->cols(), 512 );
    ASSERT_EQ( resource->rows(), 512 );
    ASSERT_EQ( resource->channels(), 3 );
    ASSERT_EQ( resource->planes(), 1 );
    ASSERT_EQ( resource->resource_name(), "GDAL" );
    ASSERT_EQ( resource->format().pixel_type(), tx::Pixel_Format_Enum::RGB );
    ASSERT_EQ( resource->format().channel_type(), tx::Channel_Type_Enum::UINT8 );
    ASSERT_EQ( resource->format().premultiply(), true );

    // Create Resource for RGBA Uint8
    tx::Image_Resource_View<tx::PixelRGBA_u8> view_01( resource );

    // Check the construction
    auto view01_res = view_01.get_constructor_status();
    if( view01_res.has_error() )
    {
        tmns::log::error( "Test about to fail: ", view01_res.error().message() );
    }
    ASSERT_FALSE( view01_res.has_error() );

    // Check attributes
    ASSERT_EQ( view_01.cols(), 512 );
    ASSERT_EQ( view_01.rows(), 512 );
    ASSERT_EQ( view_01.channels(), 4 );
    ASSERT_EQ( view_01.planes(), 1 );
    ASSERT_EQ( view_01.format().cols(), 512 );
    ASSERT_EQ( view_01.format().rows(), 512 );
    ASSERT_EQ( view_01.format().channel_type(), tx::Channel_Type_Enum::UINT8 );
    ASSERT_EQ( view_01.format().pixel_type(), tx::Pixel_Format_Enum::RGBA );

    // Build a grayscale version
    tx::Image_Resource_View<tx::PixelGray_f64> view_02( resource );
    auto view02_res = view_01.get_constructor_status();
    if( view02_res.has_error() )
    {
        tmns::log::error( "Test about to fail: ", view01_res.error().message() );
    }
    ASSERT_FALSE( view02_res.has_error() );

    // Check image attributes
    ASSERT_EQ( view_02.cols(), 512 );
    ASSERT_EQ( view_02.rows(), 512 );
    ASSERT_EQ( view_02.channels(), 1 );
    ASSERT_EQ( view_02.planes(), 1 );
    ASSERT_EQ( view_02.format().cols(), 512 );
    ASSERT_EQ( view_02.format().rows(), 512 );
    ASSERT_EQ( view_02.format().channel_type(), tx::Channel_Type_Enum::FLOAT64 );
    ASSERT_EQ( view_02.format().pixel_type(), tx::Pixel_Format_Enum::GRAY );

}