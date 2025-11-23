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
 * @file    Image_Buffer.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

// C++ Libraries
#include <cinttypes>

// Terminus Libraries
#include "Image_Format.hpp"

namespace tmns::image {

/**
 * Represents buffer data in memory.  It stores not only the raw data, but
 * characteristics about it, such as size and types.  It does not allocate
 * data, but merely references it in an organized way.
 */
class Image_Buffer
{
    public:

        /**
         * Construct buffer from format and array
        */
        Image_Buffer( Image_Format format,
                      void*        data );

        /**
         * Parameterized Constructor
        */
        Image_Buffer( void*                data,
                      const Image_Format&  format,
                      size_t               cstride,
                      size_t               rstride,
                      size_t               pstride );

        /**
         * @note Destructor needs to let the pointer go.  It's not designed
         *       to actually own the memory.
         */
        virtual ~Image_Buffer() = default;

        /**
         * Get raw data pointer
        */
        void* data() const;

        /**
         * Image Columns
        */
        size_t cols() const;

        /**
         * Image Rows
        */
        size_t rows() const;

        /**
         * Image Planes
        */
        size_t planes() const;

        /**
         * Get the pixel type
        */
        Pixel_Format_Enum pixel_type() const;

        /**
         * Set the pixel-type
        */
        void set_pixel_type( Pixel_Format_Enum value );

        /**
         * Get the channel type
        */
        Channel_Type_Enum channel_type() const;

        /**
         * Get the image format
        */
        Image_Format format() const;

        /**
         * Get reference to image format
        */
        Image_Format& format();

        /**
         * Get the column stride
        */
        ssize_t cstride() const;

        /**
         * Get the row stride
        */
        ssize_t rstride() const;

        /**
         * Get the planar stride
        */
        ssize_t pstride() const;

        /**
         * Set the pstride
        */
        void set_pstride( ssize_t value );

        /**
         * Get the point at a specific pixel
        */
        void*  operator()( int col, int row, int plane ) const;

        /**
         * Print to log-friendly string
        */
        std::string to_string( size_t offset = 0 ) const;

    private:

        /// Underlying data
        void* m_data { nullptr };

        /// Image Structure
        Image_Format m_format;

        /// Columns Stride
        ssize_t m_cstride { 0 };

        /// Row Stride
        ssize_t m_rstride { 0 };

        /// Plane Stride
        ssize_t m_pstride { 0 };

}; // End of Image_Buffer class

} // End of tmns::image namespace


    /*
    /// Returns the size (in bytes) of the data described by this buffer
    inline size_t byte_size() const {
    return planes() * pstride;
    }

    /// Returns a cropped version of this bufffer.
    inline ImageBuffer cropped( BBox2i const& bbox ) const {
    ImageBuffer self = *this;
    self.data = (uint8*)self.data + cstride*bbox.min().x() + rstride*bbox.min().y();
    self.format.cols = bbox.width();
    self.format.rows = bbox.height();
    return self;
    }

    /// Read the image resource at the given location into the given buffer.
    /// - Though the ImageBuffer object is const, the contents of the buffer will change!
    inline void read( ImageBuffer const& buf, BBox2i const& bbox ) const {
    convert( buf, cropped(bbox) );
    }

    /// Write the given buffer to the image resource at the given location.
    inline void write( ImageBuffer const& buf, BBox2i const& bbox ) {
    convert( cropped(bbox), buf );
    }

    /// Return a pointer to the pixel at (u,v,p)
    inline void* operator()( int32 i, int32 j, int32 p = 0 ) const {
    return ((uint8*)data) + (i*cstride + j*rstride + p*pstride);
    }
*/