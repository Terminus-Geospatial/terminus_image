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
 * @file    Block_Rasterize_View.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// Terminus Image Libraries
#include "../../pixel/Pixel_Accessor_Loose.hpp"
#include "../../types/Image_Base.hpp"
#include "../crop_image.hpp"
#include "Block_Generator_Manager.hpp"
#include "Block_Processor.hpp"
#include "Block_Utilities.hpp"

// Terminus Libraries
#include <terminus/core/cache/Cache_Base.hpp>
#include <terminus/math/Size.hpp>

namespace tmns::image::ops {

/**
 * Image type that overloads the rasterize method to allow processing
 * in blocks.  Creates a wrapper around the parent type.
*/
template <typename ImageT>
class Block_Rasterize_View : public Image_Base<Block_Rasterize_View<ImageT>>
{
    public:

        /// Pixel Type
        typedef typename ImageT::pixel_type pixel_type;

        /// Type returned from pixel operators
        typedef typename ImageT::pixel_type result_type;

        /// Pixel Access Type
        typedef Pixel_Accessor_Loose<Block_Rasterize_View> pixel_accessor;

        /**
         * Constructor given an image, block size, thread-count,
         * and optional cache handle
         */
        Block_Rasterize_View( io::Image_Resource_Disk::ptr_t   resource,
                              const math::Size2i&              block_size,
                              int                              num_threads = 0,
                              core::cache::Cache_Local::ptr_t  cache = nullptr )
          : m_child( std::make_shared<ImageT>( resource ) ),
            m_block_size( block_size ),
            m_num_threads( num_threads ),
            m_cache_ptr( cache )
        {
            if( m_block_size.width()  <= 0 ||
                m_block_size.height() <= 0 )
            {
                m_block_size = block::get_default_block_size<pixel_type>( resource->rows(),
                                                                          resource->cols(),
                                                                          resource->planes() );
            }

            // Manager is not needed if not using a cache.
            if (m_cache_ptr)
            {
                m_block_manager.initialize( m_cache_ptr,
                                            m_block_size,
                                            m_child );
            }
        }

        /**
         * Number of image columns
         */
        size_t cols() const
        {
            return m_child->cols();
        }

        /**
         * Number of image rows
         */
        size_t rows() const
        {
            return m_child->rows();
        }

        /**
         * Number of image planes
         */
        size_t planes() const
        {
            return m_child->planes();
        }

        /**
         * Get the origin
        */
        pixel_accessor origin() const
        {
            return pixel_accessor( *this, 0, 0, 0 );
        }

        /**
         * Rasterize a single pixel.  Not recommended.
         */
        result_type operator()( size_t x,
                                size_t y,
                                size_t p = 0 ) const
        {
            if ( m_cache_ptr )
            {
                // Note that requesting a value from a handle forces that data to be generated.
                // Early-out optimization for single-block resources
                if( m_block_manager.only_one_block() )
                {
                    const auto& handle = m_block_manager.quick_single_block();
                    result_type result = handle->operator()( x, y, p );
                    handle.release();
                    return result;
                }

                // Otherwise, figure out first what block to fetch
                auto block_index   = m_block_manager.get_block_index( tmns::math::Point2i( { (int)x, (int)y } ) );
                const auto& handle = m_block_manager.block( block_index );

                auto start_pixel = m_block_manager.get_block_start_pixel( block_index );

                // Fetch the specific entry from the handle
                result_type result = handle->operator()( x - start_pixel.x(),
                                                         y - start_pixel.y(),
                                                         p );
                handle.release();
                return result;
            } // If we have the cache handle

            // Without a cache, just load the resource directly.  Can be really f-ing slow
            else
            {
                return (*m_child)(x,y,p);
            }
        }

        /**
         * Get the Child Class
        */
        ImageT*       child()       { return *m_child; }
        const ImageT& child() const { return *m_child; }

        typedef Crop_View<Image_Memory<pixel_type> > prerasterize_type;
        prerasterize_type prerasterize( const math::Rect2i& bbox ) const
        {
            // Init output data
            Image_Memory<pixel_type> buffer( bbox.width(),
                                             bbox.height(),
                                             planes() );

            // Fill in the output data from this view
            rasterize( buffer, bbox );

            // "Fake" the bbox image so it looks like a full size image.
            return Crop_View<Image_Memory<pixel_type> >( buffer,
                                                         math::Rect2i( -bbox.min().x(),
                                                                       -bbox.min().y(),
                                                                       cols(),
                                                                       rows() ) );
        }

        template <class DestT>
        void rasterize( const DestT&        dest,
                        const math::Rect2i& bbox ) const
        {
            // Create functor to rasterize this image into the destination image
            Rasterize_Functor<DestT> rasterizer( *this, dest, bbox.min() );

            // Set up block processor to call the functor in parallel blocks.
            block::Block_Processor<Rasterize_Functor<DestT> > process( rasterizer,
                                                                       m_block_size,
                                                                       m_num_threads );

            // Tell the block processor to do all the work.
            process( bbox );
        }

        /**
         * Get this class name
        */
        static std::string class_name()
        {
            return "Block_Rasterize_View";
        }

        static std::string full_name()
        {
            return class_name() + "<" + ImageT::full_name() + ">";
        }

    private:

        /**
         * These function objects are spawned to rasterize the child image.
         * One functor is created per child thread, and they are called
         * in succession with bounding boxes that are each contained within one block.
         */
        template <typename DestT>
        class Rasterize_Functor
        {
            public:

                /**
                 * Constructor
                 * @param image
                 * @param dest
                 * @param offset
                */
                Rasterize_Functor( const Block_Rasterize_View& image,
                                   const DestT&                dest,
                                   const math::Vector2i&       offset )
                  : m_image( image ),
                    m_dest( dest ),
                    m_offset( offset )
                {}

                /**
                 * Rasterize part of m_view into m_dest.
                 */
                void operator()( const math::Rect2i& bbox ) const
                {
                    if( m_image.m_cache_ptr )
                    {
                        // Ask the cache managing object to get the image tile,
                        // we might already have it.
                        auto block_index = m_image.m_block_manager.get_block_index( bbox );

                        // Handle Type: core::cache::Cache_Local::Handle<Block_Generator<ImageT> >
                        const auto& handle = m_image.m_block_manager.block( block_index );
                        auto new_bbox = bbox - m_offset;
                        handle->rasterize( crop_image( m_dest,
                                                       new_bbox ),
                                           bbox - m_image.m_block_manager.get_block_start_pixel(block_index) );
                        handle.release();
                    }
                    // No cache, generate the image tile from scratch.
                    else
                    {
                        auto offset_bbox = bbox-m_offset;
                        m_image.child().rasterize( crop_image( m_dest, offset_bbox ), bbox );
                    }
                }

                /**
                 * Get this class name
                 */
                static std::string class_name()
                {
                    return "Rasterize_Functor";
                }

                static std::string full_name()
                {
                    return class_name() + "<" + DestT::full_name() + ">";
                }

            private:

                /// Internal Block View
                const Block_Rasterize_View& m_image;

                /// Destination Image
                const DestT& m_dest;

                /// Offset
                math::Vector2i m_offset;

        }; // End of Rasterize_Functor Class

        // Allows RasterizeFunctor to access cache-related members.
        template <typename DestT> friend class Rasterize_Functor;

        /// Child Image.  Necessary to keep local, as the block manager needs it.
        std::shared_ptr<ImageT> m_child;

        /// Block Size (in pixels)
        math::Size2i m_block_size;

        /// Number of threads to use for block processing
        int m_num_threads { 0 };

        /// Cache Handle
        core::cache::Cache_Local::ptr_t m_cache_ptr;

        /// Block-Management API
        block::Block_Generator_Manager<ImageT> m_block_manager;

}; // End of Block_Rasterize_View class

} // End of tmns::image::ops namespace