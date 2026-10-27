/**
 * @file    TEST_image_resource_disk_gdal_factory.cpp
 * @author  Marvin Smith
 * @date    7/16/2023
*/

// C++ Standard Libraries
#include <filesystem>

// GoogleTest Libraries
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

/********************************************************************/
/*          Accept Uppercase Image Extensions                       */
/********************************************************************/
TEST( io_Image_Resource_Disk_GDAL_Factory, accepts_uppercase_extension )
{
    using namespace tmns::image::io;

    const std::filesystem::path source_path( "./data/images/jpeg/lena.jpg" );
    const auto uppercase_path = std::filesystem::temp_directory_path() / "terminus_image_factory.JPG";
    std::filesystem::copy_file( source_path, uppercase_path,
                                std::filesystem::copy_options::overwrite_existing );

    const auto factory = std::make_shared<gdal::Image_Resource_Disk_GDAL_Factory>();
    EXPECT_TRUE( factory->is_read_image_supported( uppercase_path ) );

    std::filesystem::remove( uppercase_path );
}