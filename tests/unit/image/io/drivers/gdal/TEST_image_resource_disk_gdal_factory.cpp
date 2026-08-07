/**
 * @file    TEST_image_resource_disk_gdal_factory.cpp
 * @author  Marvin Smith
 * @date    7/16/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/io/drivers/gdal/image_resource_disk_gdal_factory.hpp>


/********************************************************************/
/*          Test the method for checking if a file is valid         */
/********************************************************************/
TEST( io_Image_Resource_Disk_GDAL_Factory, is_image_supported )
{
    using namespace tmns::image::io;

    auto factory = std::make_shared<gdal::Image_Resource_Disk_GDAL_Factory>();

    ASSERT_TRUE( factory->is_read_image_supported( "./data/images/jpeg/lena.jpg" ) );
    ASSERT_TRUE( factory->is_read_image_supported( "./data/images/png/lena.png" ) );
}