/**
 * @file    TEST_Image_Resource_Disk_GDAL.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/io/drivers/gdal/Image_Resource_Disk_GDAL.hpp>
#include <terminus/log/utility.hpp>

namespace tx = tmns::image;

/*********************************************************/
/*          Test the read-construction operations        */
/*********************************************************/
TEST( io_gdal_Image_Resource_Disk_GDAL, read_construction_jpg )
{
    std::filesystem::path image_to_load { "./data/images/jpeg/lena.jpg" };

    // Create a new resource
    tx::io::gdal::Image_Resource_Disk_GDAL resource( image_to_load );

    // Check it's performance
    ASSERT_EQ( resource.cols(), 512 );
    ASSERT_EQ( resource.rows(), 512 );
    ASSERT_EQ( resource.channels(), 3 );
    ASSERT_EQ( resource.planes(), 1 );
    ASSERT_EQ( resource.resource_name(), "GDAL" );
    ASSERT_EQ( resource.format().pixel_type(), tx::Pixel_Format_Enum::RGB );
    ASSERT_EQ( resource.format().channel_type(), tx::Channel_Type_Enum::UINT8 );
    ASSERT_EQ( resource.format().premultiply(), true );
}