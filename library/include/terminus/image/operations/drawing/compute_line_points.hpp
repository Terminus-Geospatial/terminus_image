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
 * @file    compute_line_points.hpp
 * @author  Marvin Smith
 * @date    8/13/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/log/utility.hpp>
#include <terminus/math/Point.hpp>
#include <terminus/math/Rectangle.hpp>

// Terminus Image Libraries
#include "../blob/Uniform_Blob.hpp"

namespace tmns::image::ops::drawing {

/**
 * Compute the points of a line for a 1-pixel thin line.
 * 
 * @param point1
 * @param point2
 * @param color
 * @param overlap_mode
 * @param[out] output
 * 
 * @returns status of the operation.
*/
template <typename PixelT>
Result<void> compute_line_points_thin( const tmns::math::Point2i&                   point1,
                                       const tmns::math::Point2i&                   point2,
                                       const PixelT&                                color,
                                       std::shared_ptr<blob::Uniform_Blob<PixelT>>& output )
{
    tmns::log::trace( ADD_CURRENT_LOC(),
                      "Start of Method.\n  P1: ", 
                      point1.to_string(),
                      "\n  P2: ", 
                      point2.to_string() );
    int error;
    auto p1 = point1;
    auto p2 = point2;

    tmns::math::Rect2i point_rect( p1,
                                   p2 );

    // If either axis is the same, then draw a box
    if( p1.x() == p2.x() ||
        p1.y() == p2.y() )
    {
        tmns::log::trace( ADD_CURRENT_LOC(), "Drawing Rectangle" );
        for( int c = point_rect.bl().x(); c <= point_rect.tr().x(); c++ )
        for( int r = point_rect.bl().y(); r <= point_rect.tr().y(); r++ )
        {
            output->insert( c, r, 0, color );
        }
    }
    else
    {
        // calculate direction
        auto delta = p2 - p1;
        tmns::math::Point2i step { { 1, 1 } };
        if( delta.x() < 0 )
        {
            delta.x() = -delta.x();
            step.x() = -1;
        }
        if( delta.y() < 0 )
        {
            delta.y() = -delta.y();
            step.y() = -1;
        }
        
        tmns::math::Point2i delta_x2( { delta.x() << 1,
                                        delta.y() << 1 } );
    
        // Add starting pixel
        output->insert( p1, color );

        if ( delta.x() > delta.y() )
        {
            // start value represents a half step in Y direction
            error = delta_x2.y() - delta.x();
        
            while ( p1.x() != p2.x() )
            {
                // step in main direction
                p1.x() += step.x();
                if( error >= 0 )
                {
                    output->insert( p1, color );

                    // change Y
                    p1.y() += step.y();
                
                    // draw pixel in minor direction before changing
                    output->insert( tmns::math::Point2i( { p1.x() - step.x(),
                                                           p1.y() } ),
                                    color );
                    error -= delta_x2.x();
                }
                error += delta_x2.y();

                output->insert( p1, color );
            }
        }
        else
        {
            error = delta_x2.x() - delta.y();
            while( p1.y() != p2.y() )
            {
                p1.y() += step.y();
                if( error >= 0 )
                {
                    output->insert( p1, color );
                    p1.x() += step.x();
                    output->insert( tmns::math::Point2i( { p1.x(),
                                                           p1.y() - step.y() } ),
                                    color );
                    error -= delta_x2.y();
                }
                error += delta_x2.x();
                output->insert( p1, color );
            }
        }
    }

    return tmns::outcome::ok();
}

/**
 * Compute the list of points which fall on a line.
 * 
 * Custom method based on computing normals of the line, in order to generate thickness.
*/
template <typename PixelT>
Result<void> compute_line_points( const math::Point2i&                         p1,
                                  const math::Point2i&                         p2,
                                  const PixelT&                                color,
                                  int                                          thickness,
                                  std::shared_ptr<blob::Uniform_Blob<PixelT>>& output )
{
    tmns::log::trace( ADD_CURRENT_LOC(),
                      "Start of Method.\n  P1: ", 
                      p1.to_string(),
                      "\n  P2: ", 
                      p2.to_string(),
                      "\n  Thickness: ",
                      thickness );

    // Make sure blob is initialized
    if( !output )
    {
        output = std::make_shared<blob::Uniform_Blob<PixelT>>( color );
    }

    // If thickness is a single line, or a fill, continue
    if( thickness == 1 )
    {
        tmns::log::trace( ADD_CURRENT_LOC(), "Drawing Thin Single-Point Line" );
        return compute_line_points_thin<PixelT>( p1,
                                                 p2,
                                                 color,
                                                 output );
    }

    // If either axis is the same, then draw a box
    if( p1.x() == p2.x() ||
        p1.y() == p2.y() )
    {
        tmns::log::trace( ADD_CURRENT_LOC(), "Drawing Big Rectangle" );

        math::Rect2i point_rect_pre( p1, p2 );
        auto point_rect = point_rect_pre.expand(  thickness / 2 );

        for( int c = point_rect.bl().x(); c <= point_rect.tr().x(); c++ )
        for( int r = point_rect.bl().y(); r <= point_rect.tr().y(); r++ )
        {
            tmns::log::trace( ADD_CURRENT_LOC(), "Adding (", c, ", ", r, ") of (", point_rect.tr().to_string(), ")" );
            output->insert( c, r, 0, color );
        }
        tmns::log::trace( ADD_CURRENT_LOC(), "All Done" );
        return outcome::ok();
    }

    // Compute a normal vector  (for 2d coordinates do [x,y] -> [y,-x])
    auto line_vector = math::Vector2f( (p2 - p1).data() );
    auto normal_full = math::Vector2f( { line_vector.y(), -line_vector.x() } );

    // Convert to unit length
    auto normal = normal_full.normalize();

    // Project on each side of each coordinate
    float half_thickness = thickness / 2.0;
    auto p1_min = p1 - ( half_thickness * normal );
    auto p1_max = p1 + ( half_thickness * normal );
    auto p1_vec = p1_max - p1_min;

    auto p2_min = p2 - ( half_thickness * normal );
    auto p2_max = p2 + ( half_thickness * normal );
    auto p2_vec = p2_max - p2_min;

    // Iterate over each line pair
    float length = std::max( 1.0, std::round( math::magnitude( p1_min - p1_max ) ) );
    log::trace( "length: ", length );

    for( int i = 0; i < length; i++ )
    {
        // Compute p1 and p2 interpolated points
        math::Point2i p1_int = p1_min + ( p1_vec * ( (float)i / length ) );
        math::Point2i p2_int = p2_min + ( p2_vec * ( (float)i / length ) );

        // Round to integer
        math::Point2i p1_render( { (int)p1_int.x(),
                                   (int)p1_int.y() } );

        math::Point2i p2_render( { (int)p2_int.x(),
                                   (int)p2_int.y() } );

        compute_line_points_thin<PixelT>( p1_render,
                                          p2_render,
                                          color,
                                          output );
    }

    return tmns::outcome::ok();
}


} // End of tmns::image::ops::drawing namespace