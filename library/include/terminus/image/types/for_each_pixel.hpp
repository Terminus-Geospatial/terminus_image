/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    for_each_pixel.hpp
 * @author  Marvin Smith
 * @date    8/24/2023
 */
#pragma once

// Terminus Image Libraries
#include "Image_Base.hpp"

// Terminus Libraries
#include <terminus/core/utility/Progress_Callback.hpp>

namespace tmns::image {


/// Function to apply a functor to each pixel of an input image.
template <typename ImageT,
          typename FunctorT>
void for_each_pixel_( const Image_Base<ImageT>&         image_,
                      FunctorT&                         func,
                      core::utility::Progress_Callback& progress )
{
    const ImageT& image = image_.impl();
    typedef typename ImageT::pixel_accessor pixel_accessor;
    pixel_accessor plane_acc = image.origin();
    for( int plane = image.planes(); plane; --plane )
    { 
        // Loop through planes
        pixel_accessor row_acc = plane_acc;
        for( int row = 0; row < image.rows(); ++row ) // Loop through rows
        { 
            progress.report_fractional_progress( row, image.rows() );
            pixel_accessor col_acc = row_acc;
            for( int col = image.cols(); col; --col ) // Loop through columns
            {
                func( *col_acc );  // Apply the functor to this pixel value
                col_acc.next_col();
            }
            row_acc.next_row();
        }
        plane_acc.next_plane();
    }
    progress.report_finished();
}

/// Overload with default no progress callback.
template <typename ImageT,
          typename FunctorT>
void for_each_pixel( const Image_Base<ImageT>&         image,
                     FunctorT&                         func,
                     core::utility::Progress_Callback& progress )
{
    for_each_pixel_<ImageT,FunctorT>( image, func, progress );
}

/// Const functor overload
template <typename ImageT,
          typename FunctorT>
void for_each_pixel( const Image_Base<ImageT>&               image,
                     const FunctorT&                         func,
                     const core::utility::Progress_Callback& progress )
{
    for_each_pixel_<ImageT,const FunctorT>( image, func, progress );
}

/// Overload for applying a functor to two input images.
template <typename Image1T,
          typename Image2T,
          typename FunctorT>
void for_each_pixel_( const Image_Base<Image1T>& image1_,
                      const Image_Base<Image2T>& image2_,
                      FunctorT&                  func )
{
    const Image1T& image1 = image1_.impl();
    const Image2T& image2 = image2_.impl();

    if( image1.cols()   != image2.cols() ||
        image1.rows()   != image2.rows() ||
        image1.planes() != image2.planes() )
    {
        std::stringstream sout;
        sout << "for_each_pixel_: Image arguments must have the same dimensions.";
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }
    typedef typename Image1T::pixel_accessor pixel_accessor_1;
    typedef typename Image2T::pixel_accessor pixel_accessor_2;
    pixel_accessor_1 plane_acc_1 = image1.origin();
    pixel_accessor_2 plane_acc_2 = image2.origin();
    for( int plane = image1.planes(); plane; --plane )
    {
        pixel_accessor_1 row_acc_1 = plane_acc_1;
        pixel_accessor_2 row_acc_2 = plane_acc_2;
        for( int row = image1.rows(); row; --row )
        {
            pixel_accessor_1 col_acc_1 = row_acc_1;
            pixel_accessor_2 col_acc_2 = row_acc_2;
            for( int col = image1.cols(); col; --col )
            {
                func( *col_acc_1, *col_acc_2 );
                col_acc_1.next_col();
                col_acc_2.next_col();
            }
            row_acc_1.next_row();
            row_acc_2.next_row();
        }
        plane_acc_1.next_plane();
        plane_acc_2.next_plane();
    }
}

template <typename Image1T,
          typename Image2T,
          typename FunctorT>
void for_each_pixel( const Image_Base<Image1T>& image1,
                     const Image_Base<Image2T>& image2,
                     FunctorT&                  func )
{ 
    for_each_pixel_<Image1T,Image2T,FunctorT>( image1, image2, func );
}

template <typename Image1T,
          typename Image2T,
          typename FunctorT>
void for_each_pixel( const Image_Base<Image1T>& image1,
                     const Image_Base<Image2T>& image2,
                     const FunctorT&            func )
{
    for_each_pixel_<Image1T,Image2T,const FunctorT>( image1, image2, func );
}

/// Overload for applying a functor to three input images.
template <typename Image1T,
          typename Image2T,
          typename Image3T,
          typename FunctorT>
void for_each_pixel_( const Image_Base<Image1T>&  image1_,
                      const Image_Base<Image2T>&  image2_,
                      const Image_Base<Image3T>&  image3_,
                      FunctorT&                   func )
{
    const Image1T& image1 = image1_.impl();
    const Image2T& image2 = image2_.impl();
    const Image3T& image3 = image3_.impl();

    if( image1.cols()   != image2.cols() ||
        image1.cols()   != image3.cols() ||
        image1.rows()   != image2.rows() ||
        image1.rows()   != image3.rows() ||
        image1.planes() != image2.planes() ||
        image1.planes() != image3.planes() )
    {
        std::stringstream sout;
        sout << "for_each_pixel_: Images must all have same dimensions";
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }
    
    typedef typename Image1T::pixel_accessor pixel_accessor_1;
    typedef typename Image2T::pixel_accessor pixel_accessor_2;
    typedef typename Image3T::pixel_accessor pixel_accessor_3;
    pixel_accessor_1 plane_acc_1 = image1.origin();
    pixel_accessor_2 plane_acc_2 = image2.origin();
    pixel_accessor_3 plane_acc_3 = image3.origin();
    for( int plane = image1.planes(); plane; --plane )
    {
        pixel_accessor_1 row_acc_1 = plane_acc_1;
        pixel_accessor_2 row_acc_2 = plane_acc_2;
        pixel_accessor_3 row_acc_3 = plane_acc_3;
        for( int row = image1.rows(); row; --row )
        {
            pixel_accessor_1 col_acc_1 = row_acc_1;
            pixel_accessor_2 col_acc_2 = row_acc_2;
            pixel_accessor_3 col_acc_3 = row_acc_3;
            for( int col = image1.cols(); col; --col )
            {
                func( *col_acc_1, *col_acc_2, *col_acc_3 );
                col_acc_1.next_col();
                col_acc_2.next_col();
                col_acc_3.next_col();
            }
            row_acc_1.next_row();
            row_acc_2.next_row();
            row_acc_3.next_row();
        }
        plane_acc_1.next_plane();
        plane_acc_2.next_plane();
        plane_acc_3.next_plane();
    }
}

template <typename Image1T,
          typename Image2T,
          typename Image3T,
          typename FunctorT>
void for_each_pixel( const Image_Base<Image1T>& image1,
                     const Image_Base<Image2T>& image2,
                     const Image_Base<Image3T>& image3,
                     FunctorT&                  func )
{
    for_each_pixel_<Image1T,Image2T,Image3T,FunctorT>( image1, image2, image3, func );
}


template <typename Image1T,
          typename Image2T,
          typename Image3T,
          typename FunctorT>
void for_each_pixel( const Image_Base<Image1T>& image1,
                     const Image_Base<Image2T>& image2,
                     const Image_Base<Image3T>& image3,
                     const FunctorT&            func )
{
    for_each_pixel_<Image1T,Image2T,Image3T,const FunctorT>( image1, image2, image3, func );
}


} // End of tmns::image namespace