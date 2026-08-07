/**
 * @file    TEST_find_keypoints.cpp
 * @author  Marvin Smith
 * @date    7/20/2023
*/
#include <gtest/gtest.h>

// C++ Libraries
#include <filesystem>

// Terminus Libraries
#include <terminus/feature/drivers/ocv/config/detector_config_ocv_orb.hpp>
#include <terminus/feature/detect_interest_points.hpp>
#include <terminus/image/io/read_image_disk.hpp>
#include <terminus/image/pixel/pixel_rgba.hpp>
#include <terminus/image/utility/view_utilities.hpp>

// Unit-Test APIs
#include "../UNIT_TEST_ONLY/Test_Environment.hpp"

namespace tx=tmns::image;
namespace tf=tmns::feature;

/************************************************************/
/*          Test the Find Keypoints on a test image         */
/*          - Disk Image Example                            */
/************************************************************/
TEST( detect_interest_points, image_disk_example_01_ocv_orb )
{
    // Using a NITF, load the image, then perform feature detection on it
    std::filesystem::path image_to_load( "data/images/jpeg/lena.jpg" );

    auto image = tx::io::read_image_disk<tx::PixelRGB_u8>( image_to_load );
    if( image.has_error() )
    {
        tmns::log::error( "Test about to fail: ",
                          image.error().message() );
    }
    ASSERT_FALSE( image.has_error() );

    // Get the components
    auto& disk_image      = image.value();
    if( Test_Environment::use_gui() )
    {
        tx::utility::view_image( "Image", disk_image );
    }
    auto detector_config = std::make_shared<tf::ocv::Detector_Config_OCV_ORB>();
    auto session_context = tmns::core::create_default_session();

    // Run ORB detector
    auto result = tf::detect_interest_points( disk_image,
                                              detector_config,
                                              session_context );

    if( result.has_error() )
    {
        tmns::log::error( "Test about to fail: ",
                          result.error().message() );
    }

    // Verify the operation was successful
    ASSERT_FALSE( result.has_error() );

    // Verify the image has interest points
    ASSERT_TRUE( disk_image.interest_points().size() > 50 );
}

/************************************************************/
/*          Test the Find Keypoints on a test image         */
/*          - Disk Image Example                            */
/************************************************************/
TEST( detect_interest_points, image_disk_example_02_ocv_gftt )
{
    // Using a NITF, load the image, then perform feature detection on it
    std::filesystem::path image_to_load( "data/images/jpeg/lena.jpg" );

    auto image = tx::io::read_image_disk<tx::PixelRGB_u8>( image_to_load );
    if( image.has_error() )
    {
        tmns::log::error( "Test about to fail: ",
                          image.error().message() );
    }
    ASSERT_FALSE( image.has_error() );

    // Get the components
    auto& disk_image      = image.value();
    if( Test_Environment::use_gui() )
    {
        tx::utility::view_image( "Image", disk_image );
    }

    auto detector_config = std::make_shared<tf::ocv::Detector_Config_OCV_GFTT>();
    auto session_context = tmns::core::create_default_session();

    // Run ORB detector
    auto result = tf::detect_interest_points( disk_image,
                                              detector_config,
                                              session_context );

    if( result.has_error() )
    {
        tmns::log::error( "Test about to fail: ",
                          result.error().message() );
    }

    // Verify the operation was successful
    ASSERT_FALSE( result.has_error() );

    // Verify the image has interest points
    ASSERT_TRUE( disk_image.interest_points().size() > 50 );
}