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
#include <terminus/image/types/image_format.hpp>

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
         * @brief Construct buffer from format and array
         *
         * @param format Image format
         * @param data Raw data pointer
         */
        Image_Buffer( Image_Format format,
                      void*        data );

        /**
         * @brief Parameterized Constructor
         *
         * @param data Raw data pointer
         * @param format Image format
         * @param cstride Column stride
         * @param rstride Row stride
         * @param pstride Plane stride
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
         * @brief Get raw data pointer
         *
         * @return void* Raw data pointer
         */
        void* data() const;

        /**
         * @brief Image Columns
         *
         * @return int Image columns
         */
        int cols() const;

        /**
         * @brief Image Rows
         *
         * @return int Image rows
         */
        int rows() const;

        /**
         * @brief Image Planes
         *
         * @return int Image planes
         */
        int planes() const;

        /**
         * @brief Get the pixel type
         *
         * @return Pixel_Format_Enum Pixel type
         */
        Pixel_Format_Enum pixel_type() const;

        /**
         * @brief Set the pixel-type
         *
         * @param value New pixel type
         */
        void set_pixel_type( Pixel_Format_Enum value );

        /**
         * @brief Get the channel type
         *
         * @return Channel_Type_Enum Channel type
         */
        Channel_Type_Enum channel_type() const;

        /**
         * @brief Get the image format
         *
         * @return Image_Format Copy of the underlying image format
         */
        Image_Format format() const;

        /**
         * @brief Get reference to image format
         *
         * @return Image_Format& Mutable reference to the underlying image format
         */
        Image_Format& format();

        /**
         * @brief Get the column stride
         *
         * @return ssize_t Column stride, in bytes
         */
        ssize_t cstride() const;

        /**
         * @brief Get the row stride
         *
         * @return ssize_t Row stride, in bytes
         */
        ssize_t rstride() const;

        /**
         * @brief Get the planar stride
         *
         * @return ssize_t Plane stride, in bytes
         */
        ssize_t pstride() const;

        /**
         * @brief Set the pstride
         *
         * @param value New plane stride, in bytes
         */
        void set_pstride( ssize_t value );

        /**
         * @brief Get the point at a specific pixel
         *
         * @param col Pixel column
         * @param row Pixel row
         * @param plane Pixel plane
         * @return void* Pointer to the pixel data at the given location
         */
        void*  operator()( int col, int row, int plane ) const;

        /**
         * @brief Print to log-friendly string
         *
         * @param offset Number of spaces to indent each line
         * @return std::string Formatted, log-friendly representation
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