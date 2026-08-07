/**
 * @file    TEST_crop_image.cpp
 * @author  Marvin Smith
 * @date    7/29/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/operations/crop_image.hpp>
#include <terminus/image/types/image_memory.hpp>
#include <terminus/image/utility/view_utilities.hpp>
#include <terminus/math/rectangle.hpp>

// Terminus Unit-Test Libraries
#include "../../UNIT_TEST_ONLY/Prerasterization_Test_View.hpp"

namespace tx = tmns::image;

/********************************************/
/*          Test the Crop View Class        */
/********************************************/

TEST( ops_Crop_View, image_memory_test )
{
    // Create an image with some dummy values
    tx::Image_Memory<uint16_t> image_01( 500, 500 );

    // Crop window
    tmns::math::Rect2i bbox( 200, 200, 200, 200 );

    // Image gets 20 pixel blocks going from 0-1, multiplied by a max possible value
    int block_size = 20;
    double step_size = 0.01;
    double max_val = ( image_01.cols() / block_size ) + ( image_01.rows() / block_size );

    // A random set of pixel values get saved
    std::vector<std::tuple<tmns::math::Point2i,uint16_t>> test_points;
    for( int r = 0; r < image_01.rows(); r++ )
    for( int c = 0; c < image_01.cols(); c++ )
    {
        double p0 = ( ( r / block_size ) + ( c / block_size ) ) / max_val;

        uint16_t pval = std::numeric_limits<uint16_t>::max() * p0;

        if( rand() % 1000 < 100 && bbox.is_inside( tmns::math::Point2_<int>( { c, r } ) ) )
        {
            test_points.push_back( std::make_tuple( tmns::math::Point2i( { c, r } ), pval ) );
        }

        image_01( r, c, 0 ) = pval;
    }

    if( test_points.empty() )
    {
        tmns::log::fatal( "Wow I did the math wrong here! Please re-run this test." );
        FAIL();
    }

    // Wrap with a crop-view
    tx::ops::Crop_View<tx::Image_Memory<uint16_t> > crop_view_01( image_01, bbox );

    ASSERT_EQ( crop_view_01.cols(), 200 );
    ASSERT_EQ( crop_view_01.rows(), 200 );
    ASSERT_EQ( crop_view_01.planes(), 1 );

    // Test individual pixel access
    for( const auto& tp : test_points )
    {
        // We need to offset the test point by the bbox's lower-left corner, since this is a crop
        // and thus the origin is now different.
        auto pt = std::get<0>( tp ) - bbox.bl();
        ASSERT_EQ( crop_view_01( pt.x(), pt.y() ),
                   std::get<1>( tp ) );
    }

    // Test full rasterization
    tx::Image_Memory<uint16_t> image_02 = crop_view_01;
    ASSERT_EQ( image_02.cols(),   crop_view_01.cols() );
    ASSERT_EQ( image_02.rows(),   crop_view_01.rows() );
    ASSERT_EQ( image_02.planes(), crop_view_01.planes() );

    for ( int r = 0; r < image_02.rows(); ++r )
    for ( int c = 0; c < image_02.cols(); ++c )
    {
        ASSERT_EQ( image_02( c, r ), crop_view_01( c, r ) );
    }

    // Test partial rasterization
    tx::Image_Memory<uint16_t> image_03( 100, 100 );

    tmns::math::Rect2i bbox_03( 50, 50, image_03.cols(), image_03.rows() );
    ASSERT_NO_THROW( { crop_view_01.rasterize( image_03,
                                               bbox_03 ); } );

    // Test partial rasterization
    for ( int r = 0; r < image_03.rows(); ++r )
    for ( int c = 0; c < image_03.cols(); ++c )
    {
        ASSERT_EQ( image_03(c,r), crop_view_01( c + 50, r + 50 ) );
    }

    // Test prerasterization
    Prerasterization_Test_View<uint16_t> prerasterized_view( 4, 4 );

    tx::ops::Crop_View<Prerasterization_Test_View<uint16_t>> crop_view_02( prerasterized_view, 0, 1, 3, 3 );

    ASSERT_NO_THROW( crop_view_02.prerasterize( tmns::math::Rect2i( 2, 0, 1, 2 ) ) );

    ASSERT_NEAR( prerasterized_view.bbox().min().x(), 2, 0.001 );
    ASSERT_NEAR( prerasterized_view.bbox().min().y(), 1, 0.001 );
    ASSERT_NEAR( prerasterized_view.bbox().max().x(), 2, 0.001 );
    ASSERT_NEAR( prerasterized_view.bbox().max().y(), 2, 0.001 );

    /*
    // Test the accessor / generic rasterization
    tx::Image_Memory<uint8_t> im4(cv.cols(),cv.rows());
  vw::rasterize( cv, im4, BBox2i(0,0,cv.cols(),cv.rows()) );
  for ( int r=0; r<im2.rows(); ++r )
    for ( int c=0; c<im3.cols(); ++c )
      EXPECT_EQ( im4(c,r), cv(c,r) );

  // Test the iterator
  ImageView<double>::iterator im2i = im2.begin();
  CropView<ImageView<double> >::iterator cvi = cv.begin();
  for ( int i=0; i<im2.cols()*im2.rows(); ++i ) {
    EXPECT_GT( cv.end() - cvi, 0 );
    EXPECT_EQ( *im2i, *cvi );
    ASSERT_NO_THROW( ++cvi );
    ++im2i;
  }
  EXPECT_EQ( cv.end() - cvi, 0 );

  // Test the types
  ASSERT_TRUE( has_pixel_type<double>( cv ) );
  ASSERT_TRUE( bool_trait<IsMultiplyAccessible>(cv) );
  ASSERT_TRUE( bool_trait<IsImageView>(cv) );
  */
}

/*
TEST( Manipulation, Crop ) {
  ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
  ImageView<double> im2 = crop(im,1,1,1,2);
  ASSERT_EQ( im2.cols(), 1 );
  ASSERT_EQ( im2.rows(), 2 );
  EXPECT_EQ( im2(0,0), 4 );
  EXPECT_EQ( im2(0,1), 6 );

  ImageView<double> im3 = crop(im,BBox2i(1,1,1,2));
  ASSERT_EQ( im3.cols(), 1 );
  ASSERT_EQ( im3.rows(), 2 );
  EXPECT_EQ( im3(0,0), 4 );
  EXPECT_EQ( im3(0,1), 6 );

  // Make sure it's really shallow.
  EXPECT_EQ( crop(im,1,1,1,2)(0,0), im(1,1) );
  EXPECT_EQ( &(crop(im,1,1,1,2)(0,0)), &(im(1,1)) );

  // Test the traits
  ASSERT_TRUE( bool_trait<IsMultiplyAccessible>( crop(im,1,1,1,2) ) );
}

TEST( Manipulation, CropAssignment ) {
  ImageView<int> dest(3,3);
  fill(dest,1);
  ImageView<int> a(2,2);
  a(0,0) = 1; // A|13|
  a(0,1) = 2; //  |24|
  a(1,0) = 3;
  a(1,1) = 4;

  crop(dest,1,1,2,2) = a;
  EXPECT_EQ( 1, dest(0,0) );
  EXPECT_EQ( 1, dest(1,0) );
  EXPECT_EQ( 1, dest(2,0) );
  EXPECT_EQ( 1, dest(0,1) );
  EXPECT_EQ( 1, dest(1,1) );
  EXPECT_EQ( 3, dest(2,1) );
  EXPECT_EQ( 1, dest(0,2) );
  EXPECT_EQ( 2, dest(1,2) );
  EXPECT_EQ( 4, dest(2,2) );

  crop(dest,1,0,2,1) = crop(a,0,1,2,1);
  EXPECT_EQ( 1, dest(0,0) );
  EXPECT_EQ( 2, dest(1,0) );
  EXPECT_EQ( 4, dest(2,0) );
  EXPECT_EQ( 1, dest(0,1) );
  EXPECT_EQ( 1, dest(1,1) );
  EXPECT_EQ( 3, dest(2,1) );
  EXPECT_EQ( 1, dest(0,2) );
  EXPECT_EQ( 2, dest(1,2) );
  EXPECT_EQ( 4, dest(2,2) );
}
*/