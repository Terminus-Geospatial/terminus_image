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
 * @file    View_Utilities.hpp
 * @author  Marvin Smith
 * @date    7/10/2023
*/
#pragma once

// Terminus Image Libraries
#include "../operations/drawing/drawing_functions.hpp"
#include "../operations/pixel_cast.hpp"
#include "../types/Image_Base.hpp"
#include "../types/Image_Buffer.hpp"
#include "../types/Image_Memory.hpp"

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

namespace tmns::image::utility {

/**
 * Render the actual scene.
*/
Result<void> visualize( const std::string&  window_name,
                        const Image_Buffer& buffer_data,
                        int                 window_sleep = 0 );

/**
 * Simple test tool for viewing an image
*/
template <typename PixelT>
Result<void> view_image( const std::string&          window_name,
                         const Image_Memory<PixelT>& image,
                         int                         window_sleep = 0,
                         bool                        draw_interest_points = false,
                         typename math::Compound_Channel_Cast<PixelT,uint8_t>::type  draw_color = Channel_Range<typename math::Compound_Channel_Type<typename math::Compound_Channel_Cast<PixelT,uint8_t>::type>::type>::max() )
{
    // We need all images to be uint8
    typedef typename math::Compound_Channel_Cast<PixelT,uint8_t>::type DestPixelT;
    Image_Memory<DestPixelT> temp = ops::pixel_cast_rescale<DestPixelT>( image );

    // If we are drawing interest points, do it here
    if( draw_interest_points )
    {
        // Create a blob to write to.
        auto uniform_blob = std::make_shared<ops::blob::Uniform_Blob<DestPixelT>>( draw_color );
        
        // Draw circles for every feature-point
        for( const auto& ip : image.interest_points() )
        {
            auto res = ops::drawing::compute_circle_points( ip.raster_loc(),
                                                            5,
                                                            draw_color,
                                                            2,
                                                            5,
                                                            uniform_blob );
        }

        // Render the image
        std::deque<std::shared_ptr<ops::blob::Uniform_Blob<DestPixelT>>> blobs;
        blobs.push_back( uniform_blob );

        auto sparse_img = ops::blob::Sparse_Image_View<Image_Memory<DestPixelT>,
                                                       ops::blob::Uniform_Blob<DestPixelT>>( temp, 
                                                                                             blobs );

        temp = sparse_img;
    }

    auto result = visualize( window_name,
                             temp.buffer(),
                             window_sleep );

    return result;
}

/**
 * View any image
*/
template <typename ImageT>
Result<void> view_image( const std::string&  window_name,
                         const ImageT&       image,
                         int                 window_sleep = 0,
                         bool                draw_interest_points = false,
                         typename math::Compound_Channel_Cast<typename ImageT::pixel_type,
                                                                   uint8_t>::type  draw_color = Channel_Range<typename math::Compound_Channel_Type<typename math::Compound_Channel_Cast<typename ImageT::pixel_type,uint8_t>::type>::type>::max() )
{
    tmns::log::info( ADD_CURRENT_LOC(), "Input Image Traits: ", image.format().to_string() );
    // Convert to Image_Memory
    Image_Memory<typename math::Compound_Channel_Cast<typename ImageT::pixel_type,uint8_t>::type> temp;
    temp = image;
    return view_image( window_name, 
                       temp,
                       window_sleep,
                       draw_interest_points,
                       draw_color );
}

} // End of tmns::image::utility namespace