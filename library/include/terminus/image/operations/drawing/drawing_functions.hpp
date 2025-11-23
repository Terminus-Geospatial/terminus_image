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
 * @file    draw_line.hpp
 * @author  Marvin Smith
 * @date    8/13/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/Point.hpp>

// Terminus Image Libraries
#include "../blob/Sparse_Image_View.hpp"
#include "../blob/Uniform_Blob.hpp"
#include "compute_circle_points.hpp"
#include "compute_line_points.hpp"

// C++ Libraries
#include <deque>

namespace tmns::image::ops {

/**
 * Draw a line on the image
 * 
 * @param input_image
 * @param p1
 * @param p2
 * @param color
 * @param thickness
 * 
 * @returns Image with line drawn on it.
*/
template <typename ImageT>
blob::Sparse_Image_View<ImageT,blob::Uniform_Blob<typename ImageT::pixel_type>> 
        draw_line( const ImageT&                      input_image,
                   const tmns::math::Point2i&         p1,
                   const tmns::math::Point2i&         p2,
                   const typename ImageT::pixel_type& color,
                   int                                thickness )
{
    typedef typename ImageT::pixel_type PixelT;

    // Construct list of coordinates
    auto uniform_blob = std::make_shared<blob::Uniform_Blob<PixelT>>( color );
    auto res = drawing::compute_line_points( p1,
                                             p2,
                                             color,
                                             thickness,
                                             uniform_blob );

    // Build the Sparse Image View
    std::deque<std::shared_ptr<blob::Uniform_Blob<PixelT>>> blobs;
    blobs.push_back( uniform_blob );

    return blob::Sparse_Image_View<ImageT,
                                   blob::Uniform_Blob<PixelT>>( input_image, 
                                                                blobs );
}

/**
 * Draw a circle on the image
 * 
 * @param input_image
 * @param center
 * @param radius
 * @param color
 * @param thickness
 * 
 * @returns Image with line drawn on it.
*/
template <typename ImageT>
blob::Sparse_Image_View<ImageT,blob::Uniform_Blob<typename ImageT::pixel_type>> 
        draw_circle( const ImageT&                      input_image,
                     const tmns::math::Point2i&         center,
                     double                             radius,
                     const typename ImageT::pixel_type& color,
                     int                                thickness,
                     int                                max_circle_segment_length = 10 )
{
    typedef typename ImageT::pixel_type PixelT;

    // Construct list of coordinates
    auto uniform_blob = std::make_shared<blob::Uniform_Blob<PixelT>>( color );
    auto res = drawing::compute_circle_points( center,
                                               radius,
                                               color,
                                               thickness,
                                               max_circle_segment_length,
                                               uniform_blob );

    // Build the Sparse Image View
    std::deque<std::shared_ptr<blob::Uniform_Blob<PixelT>>> blobs;
    blobs.push_back( uniform_blob );

    return blob::Sparse_Image_View<ImageT,
                                   blob::Uniform_Blob<PixelT>>( input_image, 
                                                                blobs );
}

/**
 * Draw a circle on the image
 * 
 * @param input_image
 * @param center
 * @param radius
 * @param color
 * @param thickness
 * 
 * @returns Image with line drawn on it.
*/
template <typename ImageT>
blob::Sparse_Image_View<ImageT,blob::Uniform_Blob<typename ImageT::pixel_type>> 
        draw_rectangle( const ImageT&                      input_image,
                        const tmns::math::Rect2i&          bbox,
                        const typename ImageT::pixel_type& color,
                        int                                thickness )
{
    typedef typename ImageT::pixel_type PixelT;

    tmns::log::info( "Drawing Rectangle. Bbox: ", bbox.to_string() );

    // Construct list of coordinates
    auto uniform_blob = std::make_shared<blob::Uniform_Blob<PixelT>>( color );

    // If the thickness is < 0, then it's a simple fill
    if( thickness < 0 )
    {
        // compute bounds
        auto new_bbox = bbox.expand( thickness / 2 );

        for( int x = new_bbox.bl().x(); x < new_bbox.tr().x(); x++ )
        for( int y = new_bbox.bl().y(); y < new_bbox.tr().y(); y++ )
        {
            uniform_blob->insert( x, y, 0, color );
        }
    }
    else
    {
        // TL -> TR
        {
            tmns::log::info( "TL: ", bbox.tl().to_string() );
            tmns::log::info( "TR: ", bbox.tr().to_string() );
            auto res = drawing::compute_line_points( bbox.tl(),
                                                     bbox.tr(),
                                                     color,
                                                     thickness,
                                                     uniform_blob );
        }

        // TR -> BR
        {
            auto res = drawing::compute_line_points( bbox.tr(),
                                                     bbox.br(),
                                                     color,
                                                     thickness,
                                                     uniform_blob );
        }

        // BR -> BL
        {
            auto res = drawing::compute_line_points( bbox.br(),
                                                     bbox.bl(),
                                                     color,
                                                     thickness,
                                                     uniform_blob );
        }

        // BL -> TL
        {
            auto res = drawing::compute_line_points( bbox.bl(),
                                                     bbox.tl(),
                                                     color,
                                                     thickness,
                                                     uniform_blob );
        }
    }

    // Build the Sparse Image View
    std::deque<std::shared_ptr<blob::Uniform_Blob<PixelT>>> blobs;
    blobs.push_back( uniform_blob );

    return blob::Sparse_Image_View<ImageT,
                                   blob::Uniform_Blob<PixelT>>( input_image, 
                                                                blobs );
}

} // End of tmns::image::ops namespace