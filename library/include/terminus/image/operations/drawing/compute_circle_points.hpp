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
 * @file    compute_circle_points.hpp
 * @author  Marvin Smith
 * @date    8/13/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/log/utility.hpp>
#include <terminus/math/Point.hpp>
#include <terminus/math/Size.hpp>

// Terminus Image Libraries
#include "compute_line_points.hpp"

namespace tmns::image::ops::drawing {

/**
 * Return a list of pixel coordinates representing the circle to draw
*/
template <typename PixelT>
Result<void> compute_circle_points( const tmns::math::Point2i&                  center,
                                    double                                      radius,
                                    const PixelT&                               color,
                                    int                                         thickness,
                                    int                                         max_circle_segment_length,
                                    std::shared_ptr<blob::Uniform_Blob<PixelT>> output )
{
    tmns::log::trace( ADD_CURRENT_LOC(),
                      "Start of Method.\n  Center: ", 
                      center.to_string(),
                      "\n  Radius: ", 
                      radius,
                      "\n  Thickness: ",
                      thickness );

    // Make sure blob is initialized
    if( !output )
    {
        output = std::make_shared<blob::Uniform_Blob<PixelT>>( color );
    }
    
    // For each segment, compute start and end points
    double angle = 0;
    double angle_step = std::atan2( max_circle_segment_length, 
                                   radius );

    math::Point2i start_pos( { (int) std::round( std::cos( angle ) * radius + center.x() ),
                               (int) std::round( std::sin( angle ) * radius + center.y() ) } );
    math::Point2i end_pos;

    while( angle < ( 2 * M_PI ) )
    {
        angle += angle_step + 0.1;

        // Create ending position
        end_pos.x() = std::round( std::cos( angle ) * radius + center.x() );
        end_pos.y() = std::round( std::sin( angle ) * radius + center.y() );

        auto dist = math::Point2i::distance( start_pos,
                                             end_pos );
        
        // Set the new line segment
        auto result = compute_line_points( start_pos,
                                           end_pos,
                                           color,
                                           thickness,
                                           output );

        // Update start pos
        start_pos = end_pos;
    }

    return tmns::outcome::ok();
}


} // End of tmns::image::ops::drawing namespace