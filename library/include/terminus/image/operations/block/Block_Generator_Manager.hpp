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
 * @file    Block_Generator_Manager.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// Terminus Image Libraries
#include "Block_Generator.hpp"

// External Terminus Libraries
#include <terminus/core/cache/Cache_Local.hpp>
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/math/Rectangle.hpp>
#include <terminus/math/Point_Utilities.hpp>
#include <terminus/math/Size.hpp>

namespace tmns::image::ops::block {

/**
 * Creates and manages blocks of data spanning the image.
 * Handles the cache API work.
*/
template <typename ImageT>
class Block_Generator_Manager
{
    public:

        /**
         * Default Constructor
        */
        Block_Generator_Manager() = default;

        /**
         * Create blocks for each region of the imagery
         */
        Result<void> initialize( core::cache::Cache_Local::ptr_t  cache,
                                 const math::Size2i&              block_size,
                                 std::shared_ptr<ImageT>          image )
        {
            // Assign the base structures
            m_cache_ptr  = cache;
            m_block_size = block_size;

            // Error checking
            if( m_block_size.width() <= 0 || m_block_size.height() <= 0 )
            {
                return outcome::fail( core::error::ErrorCode::INVALID_SIZE,
                                      "Block_Generator_Manager: Illegal block size: ",
                                      m_block_size.to_string() );
            }
            if( !m_cache_ptr )
            {
                return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                                      "Block_Generator_Manager: No cache provided!" );
            }

            // Compute Table Status
            m_table_width  = (image->cols()-1) / m_block_size.width() + 1;
            m_table_height = (image->rows()-1) / m_block_size.height() + 1;
            m_block_table.resize( m_table_height * m_table_width );
            auto view_bbox = image->full_bbox();

            // Iterate through the block positions and insert a generator object for each block
            // into m_block_table.
            for( size_t iy = 0; iy < m_table_height; ++iy ){
            for( size_t ix = 0; ix < m_table_width;  ++ix ){
                math::Rect2i bbox( ix * m_block_size.width(),
                                   iy * m_block_size.height(),
                                   m_block_size.width(),
                                   m_block_size.height() );

                bbox = math::Rect2i::intersection( bbox, view_bbox );
                block(ix,iy) = m_cache_ptr->insert( Block_Generator<ImageT>( image, bbox ) );
            }} // End loop through the blocks

            return outcome::ok();
        } // End initialize()

        /**
         * Get the block index given an input pixel coordinate
        */
        math::Point2i get_block_index( const math::Point2i& pixel ) const
        {
            return math::ToPoint2<int>( pixel.x() / m_block_size.width(),
                                        pixel.y() / m_block_size.height() );
        }

        /**
         * Get the block index for a region which must entirely fall within the bounding box
         * @note: This does not safety check.
        */
        math::Point2i get_block_index( const math::Rect2i& bbox ) const
        {
            auto block_index = get_block_index( bbox.min() );
            return block_index;
        }

        /**
         * Get the top-left pixel coordinate for a given block
         */
        math::Point2i get_block_start_pixel( const math::Point2i& block_index ) const
        {
            return math::Point2i( { block_index.x() * m_block_size.width(),
                                    block_index.y() * m_block_size.height() } );
        }

        /**
         * Make sure the block is not out of bounds
         */
        void check_block_index( const math::Point2i& block_index ) const
        {
            int ix = block_index.x();
            int iy = block_index.y();
            if( block_index.x() < 0 ||
                block_index.x() >= m_table_width ||
                block_index.y() < 0 ||
                block_index.y() >= m_table_height )
            {
                std::stringstream sout;
                sout << "BlockGeneratorManager: Block indices out of bounds, (" << ix
                     << "," << iy << ") of (" << m_table_width << "," << m_table_height << ")";
                log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }
        }


        /**
         * Get the block generator for the requested block
         */
        const core::cache::Cache_Local::Handle<Block_Generator<ImageT>>& block( const math::Point2i& block_index ) const
        {
            int ix = block_index.x();
            int iy = block_index.y();
            check_block_index(block_index);
            return m_block_table[block_index.x() + block_index.y()*m_table_width];
        }

        /**
         * Overload given x/y positions
        */
        core::cache::Cache_Local::Handle<Block_Generator<ImageT>>& block( size_t ix, size_t iy )
        {
            check_block_index( math::ToPoint2<int>( ix, iy ) );
            return m_block_table[ ix + iy * m_table_width ];
        }

        /**
         * Return true if there is only a single block
        */
        bool only_one_block() const { return ( m_block_table.size() == 1 ); }

        /**
         * Shortcut for when there is only a single block
        */
        const core::cache::Cache_Local::Handle<Block_Generator<ImageT>>& quick_single_block() const
        {
            return m_block_table[0];
        }
        core::cache::Cache_Local::Handle<Block_Generator<ImageT> >& quick_single_block()
        {
            return m_block_table[0];
        }

    private:

        /// Cache Handle
        core::cache::Cache_Local::ptr_t m_cache_ptr;

        /// Block Size
        math::Size2i m_block_size {{ 0, 0 }};

        /// Table width
        size_t m_table_width { 0 };

        /// Table Height
        size_t m_table_height { 0 };

        /// Block Table
        std::vector<core::cache::Cache_Local::Handle<Block_Generator<ImageT>>> m_block_table;

}; // End of Block_Generator_Manager class

} // End of tmns::image::ops::block namespace