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
 * @file    image_casts.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// Terminus Image Libraries
#include <terminus/image/pixel/channel_cast_utilities.hpp>
#include <terminus/image/pixel/pixel_cast_utilities.hpp>
#include <terminus/image/per_pixel_views/per_pixel_view_unary.hpp>

namespace tmns::image::ops {

/**
 * Pixel Casting Method, converting one pixel-type to another.
 * Also provides automatic rescale.
*/
template< typename PixelT,
          typename ImageT >
Per_Pixel_View_Unary<ImageT,pix::Pixel_Cast_Rescale_Functor<PixelT>> pixel_cast_rescale( const Image_Base<ImageT>& image )
{
    return Per_Pixel_View_Unary<ImageT,pix::Pixel_Cast_Rescale_Functor<PixelT>>( image.impl() );
}


/**
 * Pixel Casting Method, converting one pixel-type to another.
*/
template< typename PixelT,
          typename ImageT >
Per_Pixel_View_Unary<ImageT,pix::Pixel_Cast_Functor<PixelT>> pixel_cast( const Image_Base<ImageT>& image )
{
    return Per_Pixel_View_Unary<ImageT,pix::Pixel_Cast_Functor<PixelT>>( image.impl() );
}




} // End of tmns::image::ops namespace