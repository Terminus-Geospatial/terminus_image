/**
 * @file    TEST_Camera_Model_Factory.cpp
 * @author  Marvin Smith
 * @date    9/23/2023
 */
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/geography/camera/Camera_Model_Factory.hpp>
#include <terminus/log/utility.hpp>
#include <terminus/image/io/read_image_disk.hpp>
#include <terminus/image/pixel/Pixel_RGBA.hpp>

// Unit-Test Libraries
#include "../../UNIT_TEST_ONLY/Test_Environment.hpp"

namespace tx = tmns::image;

/********************************************/
/*          Test the ISIS loader            */
/********************************************/
TEST( geo_cam_Camera_Model_Factory, create_from_metadata_isis3 )
{
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

    // Build camera model from the metadata
    auto camera_res = tmns::geo::cam::Camera_Model_Factory::create_from_metadata( result.value().metadata() );

    ASSERT_FALSE( camera_res.has_error() );

    // Verify it's the proper type
    ASSERT_NE( camera_res.value(), nullptr );
    ASSERT_EQ( camera_res.value()->type(), "ISIS" );
}
