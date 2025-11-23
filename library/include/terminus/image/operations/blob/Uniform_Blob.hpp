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
 * @file    Uniform_Blob.hpp
 * @author  Marvin Smith
 * @date    8/13/2023
*/
#pragma once

// Terminus Image Libraries
#include "Base_Blob.hpp"

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

// C++ Libraries
#include <map>
#include <set>

namespace tmns::image::ops::blob {

template <typename PixelT>
class Uniform_Blob : public Base_Blob<Uniform_Blob<PixelT>>
{
    private:

        typedef std::map<int,std::set<int>>   ROW_MAP_TYPE;

    public:

        typedef std::shared_ptr<Uniform_Blob<PixelT>> ptr_t;

        /**
         * Construct the blob with no uniform color
         */
        Uniform_Blob() = default;

        /**
         * Construct the blob with a uniform color
         */
        Uniform_Blob( PixelT color )
          : m_color( color )
        {
        }

        /**
         * Return the number of points
         */
        size_t size() const
        {
            size_t counter = 0;
            for( const auto& row : m_pixels )
            {
                counter += row.second.size();
            }
            return counter;
        }

        /**
         * Clear the blob
         */
        void clear() 
        {
            m_pixels.clear();
        }

        /**
         * Check if the pixel is inside
         */
        bool is_inside( size_t col,
                        size_t row,
                        size_t plane ) const
        {
            auto riter = m_pixels.find( row );
            if( riter == m_pixels.end() )
            {
                return false;
            }
            return ( riter->second.find( col ) != riter->second.end() );            
        }

        /**
         * Get the color registered for the pixel location
        */
        Result<PixelT> get_pixel( size_t col,
                                  size_t row,
                                  size_t plane ) const
        {
            if( !is_inside( col, row, plane ) )
            {
                return outcome::fail( core::error::ErrorCode::OUT_OF_BOUNDS,
                                      "Pixel is not registered in the blob." );
            }
            return outcome::ok<PixelT>( m_color );
        }

        /**
         * Insert an entry into the blob
         */
        void insert( size_t col,
                     size_t row,
                     size_t plane )
        {
            // Check if row has entry
            if( m_pixels.find( row ) == m_pixels.end() )
            {
                m_pixels[row] = std::set<int>();
            }
            m_pixels[row].insert( col );
        }

        /**
         * Insert an entry into the blob
         * 
         * @param col 
         * @param row
         * @param plane
         * @param color
         * @param override_color - If set to true, color is overriden internally.
         * 
         * Besides optionally setting the color, this method is a pass-through to the
         * primary insert method.
         */
        void insert( size_t col,
                     size_t row,
                     size_t plane,
                     const PixelT& color,
                     bool          override_color = false )
        {
            // check if we need to override color
            if( override_color )
            {
                m_color = color;
            }
            insert( col, row, plane );
        }

        /**
         * Insert an entry into the blob
         * 
         * @param pixel_loc
         * @param color
         * @param override_color - If set to true, color is overriden internally.
         * 
         * Besides optionally setting the color, this method is a pass-through to the
         * primary insert method.
         */
        void insert( const tmns::math::Point2i& pixel_loc,
                     const PixelT&              color,
                     bool                       override_color = false )
        {
            insert( pixel_loc.x(),
                    pixel_loc.y(),
                    0,
                    color,
                    override_color );
        }

        /**
         * Get list of pixels
        */
        std::vector<math::Point2i> get_pixel_list() const
        {
            std::vector<math::Point2i> pixels;
            for( const auto& row : m_pixels )
            {
                for( const auto& col : row.second )
                {
                    pixels.push_back( math::Point2i( { col, row.first } ) );
                }
            }
            return pixels;
        }

    private:

        /// Color
        PixelT m_color;

        /// List of pixels
        ROW_MAP_TYPE m_pixels;

}; // End of Uniform_Blob class

} // End of tmns::image::ops::blob namespace