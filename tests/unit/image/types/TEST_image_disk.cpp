/**
 * @file    TEST_Image_Disk.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/Pixel_Gray.hpp>
#include <terminus/image/pixel/Pixel_RGBA.hpp>
#include <terminus/image/types/Image_Disk.hpp>
#include <terminus/log/utility.hpp>

namespace tx = tmns::image;

/****************************************************/
/*      Test Construction using GDAL Resource       */
/****************************************************/
TEST( types_Image_Disk, construct_gdal_resource_jpg )
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

    // Build the disk image memory cache
    auto cache = std::make_shared<tmns::core::cache::Cache_Local>( 1000000000 );

    // Build the disk image itself
    tx::Image_Disk<tx::PixelRGBA_u8> disk_image_01( resource, cache );

    // Check image attributes
    ASSERT_EQ( disk_image_01.cols(), 512 );
    ASSERT_EQ( disk_image_01.rows(), 512 );
    ASSERT_EQ( disk_image_01.channels(), 4 );
    ASSERT_EQ( disk_image_01.planes(), 1 );
    ASSERT_EQ( disk_image_01.format().cols(), 512 );
    ASSERT_EQ( disk_image_01.format().rows(), 512 );
    ASSERT_EQ( disk_image_01.format().channel_type(), tx::Channel_Type_Enum::UINT8 );
    ASSERT_EQ( disk_image_01.format().pixel_type(), tx::Pixel_Format_Enum::RGBA );

    // Build a grayscale version
    tx::Image_Disk<tx::PixelGray_f64> disk_image_02( resource, cache );

    // Check image attributes
    ASSERT_EQ( disk_image_02.cols(), 512 );
    ASSERT_EQ( disk_image_02.rows(), 512 );
    ASSERT_EQ( disk_image_02.channels(), 1 );
    ASSERT_EQ( disk_image_02.planes(), 1 );
    ASSERT_EQ( disk_image_02.format().cols(), 512 );
    ASSERT_EQ( disk_image_02.format().rows(), 512 );
    ASSERT_EQ( disk_image_02.format().channel_type(), tx::Channel_Type_Enum::FLOAT64 );
    ASSERT_EQ( disk_image_02.format().pixel_type(), tx::Pixel_Format_Enum::GRAY );
}