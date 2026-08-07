/**
 * @file    TEST_select_plane.cpp
 * @author  Marvin Smith
 * @date    7/29/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/operations/select_plane.hpp>
#include <terminus/image/types/image_memory.hpp>
#include <terminus/image/utility/view_utilities.hpp>

namespace tx = tmns::image;

/****************************************************/
/*          Quick check of Select_Plane_View        */
/****************************************************/
TEST( ops_select_plane, simple_test )
{
    // Create a 2-plane double-precision image
    tx::Image_Memory<double> image_01( 500, 500, 2 );

    ASSERT_EQ( image_01.cols(),     500 );
    ASSERT_EQ( image_01.rows(),     500 );
    ASSERT_EQ( image_01.channels(), 1 );
    ASSERT_EQ( image_01.planes(),   2 );

    // Channel 0 gets 20 pixel blocks going from 0-1, Channel 1 gets whatever it takes to make the combo add to 1
    int block_size = 10;
    double step_size = 0.01;
    double max_val = ( image_01.cols() / block_size ) + ( image_01.rows() / block_size );
    for( int r = 0; r < image_01.rows(); r++ )
    for( int c = 0; c < image_01.cols(); c++ )
    {
        double p0 = ( ( r / block_size ) + ( c / block_size ) ) / max_val;
        double p1 = 1 - p0;

        image_01( r, c, 0 ) = p0;
        image_01( r, c, 1 ) = p1;
    }

    // Grab a single plane (NOTE that these are week copies until you assign it to an image-view)
    auto plane_0 = tx::ops::select_plane( image_01, 0 );
    auto plane_1 = tx::ops::select_plane( image_01, 1 );

    // Check some traits
    ASSERT_EQ( plane_0.cols(), 500 );
    ASSERT_EQ( plane_0.rows(), 500 );
    ASSERT_EQ( plane_0.planes(), 1 );

    ASSERT_EQ( plane_1.cols(), 500 );
    ASSERT_EQ( plane_1.rows(), 500 );
    ASSERT_EQ( plane_1.planes(), 1 );

    // Set a few pixels to verify
    for( size_t r = 100; r < 200; r++ )
    for( size_t c = 100; c < 200; c++ )
    {
        plane_0( c, r, 0 ) = 0;
        plane_1( c, r, 0 ) = 0;
    }

    // Verify updates hold on parent view
    for( size_t r = 0; r < image_01.rows(); r++ )
    for( size_t c = 0; c < image_01.cols(); c++ )
    {
        ASSERT_NEAR( image_01( c, r, 0 ), plane_0( c, r, 0 ), 0.0001 );
        ASSERT_NEAR( image_01( c, r, 1 ), plane_1( c, r, 0 ), 0.0001 );

        ASSERT_EQ( &(image_01( c, r, 0 )), &(plane_0( c, r, 0 )) );
        ASSERT_EQ( &(image_01( c, r, 1 )), &(plane_1( c, r, 0 )) );
    }
}