/**
 * @file    TEST_Image_Resource_Disk_GDAL_Factory.cpp
 * @author  Marvin Smith
 * @date    7/16/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/io/drivers/gdal/Image_Resource_Disk_GDAL_Factory.hpp>


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