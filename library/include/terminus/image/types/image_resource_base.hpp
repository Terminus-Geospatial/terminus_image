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
 * @file    Image_Resource_Base.hpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#pragma once

/// External Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/math/rectangle.hpp>
#include <terminus/math/size.hpp>

/// Terminus Libraries
#include <terminus/image/metadata/metadata_container_base.hpp>
#include <terminus/image/pixel/channel_type_enum.hpp>
#include <terminus/image/pixel/pixel_format_enum.hpp>
#include <terminus/image/types/image_buffer.hpp>
#include <terminus/image/types/image_format.hpp>

// C++ Libraries
#include <memory>

namespace tmns::image {

/**
 * Base-type for resources which read data from a source.
*/
class Read_Image_Resource_Base
{
    public:


        /// @brief Shared pointer type
        typedef std::shared_ptr<Read_Image_Resource_Base> ptr_t;

        /**
         * @brief Virtual destructor
         */
        virtual ~Read_Image_Resource_Base() = default;

        /**
         * @brief Get the number of columns in the image
         *
         * @return int Number of columns
         */
        virtual int cols() const;

        /**
         * @brief Get the number of rows in the image
         *
         * @return int Number of rows
         */
        virtual int rows() const;

        /**
         * @brief Get the number of planes in the image
         *
         * @return int Number of planes
         */
        virtual int planes() const;

        /**
         * @brief Get the number of channels in the image
         *
         * @return int Number of channels
         */
        virtual int channels() const;

        /**
         * @brief Get a full-size image bounding box
         *
         * @return math::Rect2i Full image bounding box
         */
        math::Rect2i full_bbox() const;

        /**
         * @brief Get the native pixel type for the image resource
         *
         * @return Pixel_Format_Enum Native pixel type
         */
        virtual Pixel_Format_Enum pixel_type() const;

        /**
         * @brief Get the underlying channel type for the image
         *
         * @return Channel_Type_Enum Underlying channel type
         */
        virtual Channel_Type_Enum channel_type() const;

        /**
         * @brief Get the image format
         *
         * @return Image_Format Image format
         */
        virtual Image_Format format() const = 0;

        /**
         * @brief Read the image data from disk
         *
         * @param dest Destination buffer
         * @param bbox Bounding box to read
         *
         * @return Result<void> Result of the read operation
         */
        virtual Result<void> read( const Image_Buffer& dest,
                                   const math::Rect2i& bbox ) const = 0;

        /**
         * @brief Check if the resource supports block reads
         *
         * @return bool True if block reads are supported
         */
        virtual bool has_block_read() const = 0;

        /**
         * @brief Return the preferred block size / alignment for partial reads
         */
        virtual math::Size2i block_read_size() const;

        /**
         * @brief Check if the resource supports nodata values for the loaded file
         *
         * @return bool True if nodata values are supported
         */
        virtual bool has_nodata_read() const = 0;

        /**
         * @brief Fetch the nodata value
         *
         * @return double Nodata value
         */
        virtual double nodata_read() const;

        /**
         * @brief Return a pointer to the data in the same format as format()
         *
         * This might cause a copy, depending on implementation. The shared_ptr will
         * handle cleanup.
         */
        virtual std::shared_ptr<const uint8_t[]> native_ptr() const;

        /**
         * @brief Get the native size of the resource
         *
         * @return size_t Native size
         */
        virtual size_t native_size() const;

}; // End of Read_Image_Resource_Base Class

/**
 * @brief Base class for write-only image resources
 */
class Write_Image_Resource_Base
{
    public:

        /// @brief Shared pointer type
        typedef std::shared_ptr<Write_Image_Resource_Base> ptr_t;

        /// @brief  Destructor
        virtual ~Write_Image_Resource_Base() = default;

        /**
         * @brief Write image data to the resource
         *
         * @param buf Source buffer
         * @param bbox Bounding box to write
         *
         * @return Result<void> Result of the write operation
         */
        virtual Result<void> write( const Image_Buffer& buf,
                                    const math::Rect2i& bbox ) = 0;


        /**
         * @brief Check if the resource supports block write operations
         *
         * @return bool True if block write operations are supported
         */
        virtual bool has_block_write() const = 0;

        /**
         * @brief Return the preferred block size / alignment for partial reads
         */
        virtual math::Size2i block_write_size() const;

        /**
         * @brief Set a specific block write size
         *
         * @param sz Block size
         */
        virtual void set_block_write_size( const math::Size2i& sz );

        /**
         * @brief Check if resource supports nodata values
         */
        virtual bool has_nodata_write() const = 0;

        /**
         * @brief Set the nodata value to be written for the file
         *
         * @param value Nodata value
         */
        virtual void set_nodata_write( double value );

        /**
         * @brief Command resource to write everything
         */
        virtual void flush() = 0;

}; // End of Write_Image_Resource_Base Class

/**
 * @brief Resource which can load and save an image
 */
class Image_Resource_Base : public Read_Image_Resource_Base,
                            public Write_Image_Resource_Base
{
    public:

        /// @brief Shared pointer type
        typedef std::shared_ptr<Image_Resource_Base> ptr_t;

        /**
         * @brief Get the internal metadata container
         */
        meta::Metadata_Container_Base::ptr_t metadata() const
        {
            return m_metadata;
        }

    protected:

        /// @brief Internal Metadata
        meta::Metadata_Container_Base::ptr_t m_metadata { std::make_shared<meta::Metadata_Container_Base>()  };
};

} // end of tmns::image namespace