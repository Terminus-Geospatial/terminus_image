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
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/math/Rectangle.hpp>
#include <terminus/math/Size.hpp>

/// Terminus Libraries
#include "../metadata/Metadata_Container_Base.hpp"
#include "../pixel/Channel_Type_Enum.hpp"
#include "../pixel/Pixel_Format_Enum.hpp"
#include "Image_Buffer.hpp"
#include "Image_Format.hpp"

// C++ Libraries
#include <memory>

namespace tmns::image {

/**
 * Base-type for resources which read data from a source.
*/
class Read_Image_Resource_Base
{
    public:

        typedef std::shared_ptr<Read_Image_Resource_Base> ptr_t;

        virtual ~Read_Image_Resource_Base() = default;

        /**
         * Get the number of columns in the image
        */
        virtual size_t cols() const;

        /**
         * Get the number of rows in the image
        */
        virtual size_t rows() const;

        /**
         * Get the number of planes in the image.
        */
        virtual size_t planes() const;

        /**
         * Get the number of channels in the image
        */
        virtual size_t channels() const;

        /**
         * Get a full-size image bounding box.
        */
        math::Rect2i full_bbox() const;

        /**
         * Get the native pixel type for the image resource
        */
        virtual Pixel_Format_Enum pixel_type() const;

        /**
         * Get the underlying channel type for the image
        */
        virtual Channel_Type_Enum channel_type() const;

        /**
         * Get the image format object
        */
        virtual Image_Format format() const = 0;

        /**
         * Read the image data from disk
        */
        virtual Result<void> read( const Image_Buffer& dest,
                                   const math::Rect2i& bbox ) const = 0;

        /**
         * Check if the resource supports block reads.
         */
        virtual bool has_block_read() const = 0;

        /**
         * Return the preferred block size / alignment for partial reads.
         */
        virtual math::Size2i block_read_size() const;

        /**
         * Check if the resource supports nodata values for the loaded file.
        */
        virtual bool has_nodata_read() const = 0;

        /**
         * Fetch the nodata value.
         */
        virtual double nodata_read() const;

        /// Return a pointer to the data in the same format as format(). This
        /// might cause a copy, depending on implementation. The shared_ptr will
        /// handle cleanup.
        virtual std::shared_ptr<const uint8_t[]> native_ptr() const;

        /**
         * Get the native size of the resource.
        */
        virtual size_t native_size() const;

}; // End of Read_Image_Resource_Base Class

class Write_Image_Resource_Base
{
    public:

        typedef std::shared_ptr<Write_Image_Resource_Base> ptr_t;

        /// @brief  Destructor
        virtual ~Write_Image_Resource_Base() = default;

        virtual Result<void> write( const Image_Buffer& buf,
                                    const math::Rect2i& bbox ) = 0;


        /**
         * Check if the resource supports block write operations.
        */
        virtual bool has_block_write() const = 0;

        /**
         * Return the preferred block size / alignment for partial reads.
         */
        virtual math::Size2i block_write_size() const;

        /**
         * Set a specific block write size
        */
       virtual void set_block_write_size(const math::Size2i& sz );

        /**
         * Check if resource supports nodata values
         */
        virtual bool has_nodata_write() const = 0;

        /**
         * Set the nodata value to be written for the file.
        */
        virtual void set_nodata_write( double value );

        /**
         * Command resource to write everything.
        */
        virtual void flush() = 0;

}; // End of Write_Image_Resource_Base Class

/**
 * Resource which can load and save an image
*/
class Image_Resource_Base : public Read_Image_Resource_Base,
                            public Write_Image_Resource_Base
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Image_Resource_Base> ptr_t;

        /**
         * @brief Get the internal metadata container
         */
        meta::Metadata_Container_Base::ptr_t metadata() const
        {
            return m_metadata;
        }

    protected:

        /// Internal Metadata
        meta::Metadata_Container_Base::ptr_t m_metadata { std::make_shared<meta::Metadata_Container_Base>()  };
};

} // end of tmns::image namespace