/**
 * @file    TEST_Detector_OCV_ORB.cpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/feature/drivers/ocv/Detector_OCV_ORB.hpp>
#include <terminus/image/io/read_image_disk.hpp>

namespace tf = tmns::feature;
namespace tx = tmns::image;

TEST( Detector_OCV_ORB, image_test_01 )
{
    // Image to process
    std::filesystem::path image_to_load { "./data/images/jpeg/lena.jpg" };
    ASSERT_TRUE( std::filesystem::exists( image_to_load ) );

    // Load in multiple formats
    tx::Image_Memory<tx::PixelGray_u8>  img_u8     = tx::io::read_image_disk<tx::PixelGray_u8>(  image_to_load ).value();
    tx::Image_Memory<tx::PixelGray_u16> img_u16    = tx::io::read_image_disk<tx::PixelGray_u16>( image_to_load ).value();
    tx::Image_Memory<tx::PixelGray_f32> img_f32    = tx::io::read_image_disk<tx::PixelGray_f32>( image_to_load ).value();
    tx::Image_Memory<tx::PixelGray_f64> img_f64    = tx::io::read_image_disk<tx::PixelGray_f64>( image_to_load ).value();
    tx::Image_Memory<tx::PixelRGB_u8>   img_rgb_u8 = tx::io::read_image_disk<tx::PixelRGB_u8>( image_to_load ).value();

    // Create feature point detector
    auto detector = std::make_shared<tf::ocv::Detector_OCV_ORB>();

    // Run the detector on each image
    {
        tmns::log::debug( ADD_CURRENT_LOC(), "testing detector on u8 image" );
        auto kp_u8 = detector->operator()( img_u8 );
        ASSERT_FALSE( kp_u8.has_error() );
        ASSERT_GT( kp_u8.assume_value().size(), 500 );
    }
    {
        tmns::log::debug( ADD_CURRENT_LOC(), "testing detector on u16 image" );
        auto kp_u16 = detector->operator()( img_u16, 1000 );
        ASSERT_FALSE( kp_u16.has_error() );
        ASSERT_GT( kp_u16.assume_value().size(), 500 );
    }
    {
        tmns::log::debug( ADD_CURRENT_LOC(), "testing detector on f32 image" );
        auto kp_f32 = detector->operator()( img_f32, 1000 );
        ASSERT_FALSE( kp_f32.has_error() );
        ASSERT_GT( kp_f32.assume_value().size(), 500 );
    }
    {
        tmns::log::debug( ADD_CURRENT_LOC(), "testing detector on f64 image" );
        auto kp_uf64 = detector->operator()( img_f64 );
        ASSERT_FALSE( kp_uf64.has_error() );
        ASSERT_GT( kp_uf64.assume_value().size(), 500 );
    }

    // Test floating point images without casting
    {
        auto kp_f32 = detector->operator()( img_f32, false );
        ASSERT_TRUE( kp_f32.has_error() );
    }
    {
        auto kp_uf64 = detector->operator()( img_f64, false );
        ASSERT_TRUE( kp_uf64.has_error() );
    }

    // Test RGB Image
    {
        tmns::log::debug( ADD_CURRENT_LOC(), "testing detector on RGB u8 image" );
        auto kp_u8 = detector->operator()( img_rgb_u8 );
        ASSERT_FALSE( kp_u8.has_error() );
        ASSERT_GT( kp_u8.assume_value().size(), 500 );

        // Visualize
        //tx::utility::view_image( "RGB U8 Keypoint Results",
        //                         tf::utility::draw_feature_points( ))

    }

}