/**
 * @file    TEST_compute_circle_points.cpp
 * @author  Marvin Smith
 * @date    8/3/2026
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/operations/drawing/compute_circle_points.hpp>
#include <terminus/image/pixel/pixel_rgb.hpp>

namespace tx = tmns::image;

/************************************************/
/*      Test Compute Simple Circle Points       */
/************************************************/
TEST( ops_drawing_compute_circle_points, compute_circle_points_simple )
{
    tmns::math::Point2i center( { 0, 0 } );
    auto color = tx::PixelRGB_u8::green();
    std::shared_ptr<tx::ops::blob::Uniform_Blob<tx::PixelRGB_u8>> output;

    auto result = tx::ops::drawing::compute_circle_points( center,
                                                           5.0,
                                                           color,
                                                           1,
                                                           1,
                                                           output );

    ASSERT_FALSE( result.has_error() );
    ASSERT_NE( output, nullptr );
    ASSERT_GT( output->get_pixel_list().size(), 0U );
}
