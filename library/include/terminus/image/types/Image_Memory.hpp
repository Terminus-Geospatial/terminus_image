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
 * @file    Image_Memory.hpp
 * @author  Marvin Smith
 * @date    7/11/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/Point.hpp>

// Terminus Image Libraries
#include "../operations/rasterize.hpp"
#include "../pixel/Pixel_Accessor_MemStride.hpp"
#include "Image_Base.hpp"
#include "Image_Buffer.hpp"
#include "Image_Resource_Base.hpp"
#include "Image_Traits.hpp"

// C++ Libraries
#include <memory>

namespace tmns::image {

/**
 * Image type for "In-Memory" operations
*/
template <typename PixelT>
class Image_Memory : public Image_Base<Image_Memory<PixelT>>
{
    public:

        /// Pixel Type
        typedef PixelT pixel_type;

        /// Return type when you query actual data
        typedef PixelT& result_type;

        /// Base type of the image
        typedef Image_Base<Image_Memory<PixelT>> base_type;

        /// Prerasterize Type
        typedef Image_Memory prerasterize_type;

        /// Accessor Type]
        typedef Pixel_Accessor_MemStride<PixelT> pixel_accessor;

        /**
         * Default Constructor
        */
        Image_Memory() = default;

        /**
         * Copy-Constructor
         *
         * Does a weak copy, which effectively transfers pointers
         * and info over.  Do not manually destruct the other
         * Image_Memory instance's pixel data.
         */
        Image_Memory( const Image_Memory& rhs )
         : Image_Base<Image_Memory<PixelT>>( rhs ),
           m_data( rhs.m_data ),
           m_cols( rhs.m_cols ),
           m_rows( rhs.m_rows ),
           m_planes( rhs.m_planes ),
           m_origin( rhs.m_origin ),
           m_rstride( rhs.m_rstride ),
           m_pstride( rhs.m_pstride )
        {}

        /**
         * Build an empty image with the default dimensions.
         * Note we set the sizes initially to zero so the `set_size()`
         * method fills everything in the "official" way. Well, we don't
         * set them here, but we let the initializers do the work.
         */
        Image_Memory( size_t cols, size_t rows, size_t planes = 1 )
        {
            set_size( cols, rows, planes );
        }

        /**
         * Build the Image from any other "Image Type". Note this
         * comes after the Copy-Constructor above so if doing an
         * Image_Memory -> Image_Memory copy, it doesn't need to call
         * the more expensive `rasterize()` method.
         */
        template <typename ImageT>
        Image_Memory( const ImageT& old_image )
          : m_cols(0),
            m_rows(0),
            m_planes(0),
            m_origin(0),
            m_rstride( 0 ),
            m_pstride(0)
        {
            set_size( old_image.cols(),
                      old_image.rows(),
                      old_image.planes() );

            old_image.rasterize( *this, old_image.full_bbox() );
        }

        /**
         * Build an Image_Memory instance from a Read_Image_Resource object.
         */
        explicit Image_Memory( const Read_Image_Resource_Base& resource )
        {
            int planes = 0;
            if(! math::Is_Compound<PixelT>::value)
            {
                // The image has a fundamental pixel type
                if( resource.planes()   > 1 &&
                    resource.channels() > 1 )
                {
                    std::stringstream sout;
                    sout << "Cannot read a multi-plane multi-channel image resource into a single-channel view.";
                    log::error( sout.str() );
                    throw std::runtime_error( sout.str() );
                }
                planes = std::max( resource.planes(), resource.channels() );
            }

            // Allocate Memory
            set_size( resource.cols(),
                      resource.rows(),
                      planes );

            // Read from the buffer
            resource.read( this->buffer(),
                           resource.full_bbox() );
        }

        /**
         * Rasterizes the input view into the image, adjusting size and
         * copying data as needed.  This returns itself.
         */
        template <typename InputImageT>
        const Image_Memory& operator = ( const Image_Base<InputImageT>& input_image )
        {
            set_size( input_image.impl().cols(),
                      input_image.impl().rows(),
                      input_image.impl().planes() );

            input_image.impl().rasterize( *this,
                                          input_image.full_bbox() );

            // Transfer other components
            this->copy_payload_data( input_image.impl() );

            return *this;
        }

        /**
         * Assignment-copy operator
         */
        template <typename InputImageT>
        const Image_Memory& operator = ( const Image_Base<InputImageT>& input_image ) const
        {
            input_image.impl().rasterize( *this,
                                          input_image.full_bbox() );

            // Transfer other components
            this->m_interest_points = input_image.m_interest_point;
            
            return *this;
        }

        /**
         * Get the number of rows in the image
        */
        size_t rows() const { return m_rows; }

        /**
         * Get the number of columns in the image
         */
        size_t cols() const { return m_cols; }

        /**
         * Get the number of planes in the image
         */
        size_t planes() const { return m_planes; }

        /**
         * Get the image origin
        */
        pixel_accessor origin() const
        {
            return pixel_accessor( m_origin,
                                   m_rstride,
                                   m_pstride );
        }
        /**
         * Return specific pixel position
         */
        result_type operator()( size_t col,
                                size_t row,
                                size_t plane = 0 ) const
        {
            return *( m_origin + col + row * m_rstride + plane * m_pstride );
        }

        /**
         * Return specific pixel position
         */
        result_type operator()( const tmns::math::Point2i& loc,
                                size_t                     plane = 0 ) const
        {
            return this->operator()( loc.x(), loc.y(), plane );
        }

        /**
         * Returns an ImageBuffer describing the image data.
         */
        Image_Buffer buffer() const
        {
            Image_Buffer buffer( data(),
                                 base_type::format(),
                                 sizeof(PixelT),
                                 sizeof(PixelT) * cols(),
                                 sizeof(PixelT) * cols() * rows() );
            return buffer;
        }

        /**
         * Get a pointer to the top-left corner of the first channel.
        */
        pixel_type* data() const
        {
            return m_data.get();
        }

        /**
         * Resize the image, allocating new memory if the size has changed.
        */
        Result<void> set_size( size_t cols,
                               size_t rows,
                               size_t planes = 1 )
        {
            // Check if already the correct size
            if( cols == m_cols && rows == m_rows && planes == m_planes )
            {
                return outcome::ok();
            }

            /// Hypothetical Max Sizes
            static const size_t MAX_PIXEL_SIZE   = 100000;
            static const size_t MAX_PLANE_COUNT  = 1024;
            static const size_t MAX_TOTAL_PIXELS = 6400000000;

            // Don't oversize
            if( cols >= MAX_PIXEL_SIZE && rows >= MAX_PIXEL_SIZE )
            {
                std::stringstream sout;
                sout << "Will not allocate more than " << MAX_PIXEL_SIZE-1
                     << " pixels on a side.";
                return outcome::fail( core::error::ErrorCode::OUT_OF_BOUNDS,
                                      sout.str() );
            }
            if( planes >= MAX_PLANE_COUNT )
            {
                std::stringstream sout;
                sout << "Will not allocate more than " << MAX_PLANE_COUNT-1
                     << " planes in the image.";
                return outcome::fail( core::error::ErrorCode::OUT_OF_BOUNDS,
                                      sout.str() );
            }

            uintmax_t num_pixels = cols * rows * planes;
            if( num_pixels >= MAX_TOTAL_PIXELS )
            {
                std::stringstream sout;
                sout << "Will not allocate more than " << MAX_TOTAL_PIXELS-1
                     << " pixels in the image.";
                return outcome::fail( core::error::ErrorCode::OUT_OF_BOUNDS,
                                      sout.str() );
            }

            if( num_pixels == 0 )
            {
                m_data.reset();
            }
            else
            {
                // I like this catch because we can wrap the result and not throw
                std::shared_ptr<PixelT[]> data( new (std::nothrow) PixelT[num_pixels] );

                if( !data )
                {
                    std::stringstream sout;
                    sout << "Cannot allocate enough memory for a " << m_cols << " x "
                         << m_rows << " x " << planes << " image.";
                    return outcome::fail( core::error::ErrorCode::OUT_OF_MEMORY,
                                          sout.str() );
                }

                m_data = data;
            }

            m_cols    = cols;
            m_rows    = rows;
            m_planes  = planes;
            m_origin  = m_data.get();
            m_rstride = cols;
            m_pstride = rows*cols;

            return outcome::ok();
        }

        void reset()
        {
            m_data.reset();
            m_cols    = 0;
            m_rows    = 0;
            m_planes  = 0;
            m_origin  = 0;
            m_rstride = 0;
            m_pstride = 0;
        }

        /**
         * Check if valid image
        */
        bool is_valid_image() const
        {
            return !(!m_data);
        }

        /**
         * Check if anyone is sharing this
        */
        bool unique() const
        {
            return (!m_data) || m_data.unique();
        }

        /**
         * Prepare an Image_Memory instance to be rasterized.  Returns the
         * original view, since memory means it's already prepped.
        */
        prerasterize_type prerasterize( const math::Rect2i& bbox ) const
        {
            return *this;
        }

        /**
         * Rasterize the image view.  Simply invokes the default
         * rasterization function.
         */
        template <class DestT>
        void rasterize( const DestT&       dest,
                       const math::Rect2i& bbox ) const
        {
            ops::rasterize( prerasterize(bbox), dest, bbox);
        }

        /**
         * Get this class name
        */
        static std::string class_name()
        {
            return "Image_Memory";
        }

        static std::string full_name()
        {
            return class_name() + "<" + math::Compound_Name<pixel_type>::name() + ">";
        }

    private:

        /// Pixel Data
        std::shared_ptr<PixelT[]> m_data;

        /// Image Traits
        size_t m_rows { 0 };
        size_t m_cols { 0 };
        size_t m_planes { 0 };

        /// Pixel Origin
        PixelT* m_origin { nullptr };

        size_t m_rstride { 0 };
        size_t m_pstride { 0 };

}; // End of Image_Memory Class

template <typename PixelT>
using Image = Image_Memory<PixelT>;


// Image_Memory traits
/// Specifies that ImageView objects are resizable.
template <class PixelT>
struct Is_Resizable<Image_Memory<PixelT>>
{
    typedef std::true_type value;
};

/// Specifies that ImageView objects are fast to access.
template <class PixelT>
struct Is_Multiply_Accessible<Image_Memory<PixelT>>
{
    typedef std::true_type value;
};


} // End of tmns::image namespace