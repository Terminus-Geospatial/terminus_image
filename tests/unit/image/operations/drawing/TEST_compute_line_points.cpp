/**
 * @file    TEST_compute_line_points.cpp
 * @author  Marvin Smith
 * @date    9/3/2023
*/
#include <gtest/gtest.h>


// Terminus Libraries
#include <terminus/image/operations/drawing/compute_line_points.hpp>
#include <terminus/image/pixel/pixel_rgb.hpp>

namespace tx=tmns::image;

TEST( ops_drawing_compute_line_points, compute_line_points_simple )
{
    tmns::math::Point2i p1( { 0, -5 } );
    tmns::math::Point2i p2( { 0,  5 } );
    auto color = tx::PixelRGB_u8::green();
    std::shared_ptr<tx::ops::blob::Uniform_Blob<tx::PixelRGB_u8>> output;

    // Define a line centered on zero with thickness of 1
    auto result = tx::ops::drawing::compute_line_points( p1,
                                                         p2,
                                                         color,
                                                         1,
                                                         output );

    ASSERT_FALSE( result.has_error() );
    ASSERT_NE( output, nullptr );

    // Check the contents
    auto act_point_list = output->get_pixel_list();

    std::vector<tmns::math::Point2i> expected_points;
    for( int i = -5; i <= 5; i++ ){ expected_points.push_back( tmns::math::Point2i( { 0, i } ) ); }

    ASSERT_EQ( expected_points.size(), act_point_list.size() );

    for( size_t i = 0; i < act_point_list.size(); i++ )
    {
        ASSERT_EQ( expected_points[i].x(),
                   act_point_list[i].x() );

        ASSERT_EQ( expected_points[i].y(),
                   act_point_list[i].y() );
    }
}