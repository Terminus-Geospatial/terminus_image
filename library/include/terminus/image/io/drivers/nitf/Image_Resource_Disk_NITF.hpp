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
 * @file    Image_Resource_Disk_NITF.hpp
 * @author  Marvin Smith
 * @date    7/19/2024
*/
#pragma once

// Terminus Libraries
#include "../../Image_Resource_Disk.hpp"

// C++ Libraries

namespace tmns::image::io::nitf {

/**
 * @class Image_Resource_Disk_NITF
 *
 * Disk Read/Write Interface for NITF.
*/
class Image_Resource_Disk_NITF : public Image_Resource_Disk
{
    public:

        /// Parent Pointer Type
        typedef Image_Resource_Disk::ptr_t ParentPtrT;

        /**
         * Parameterized Constructor for reading images.
         * @param pathname Image to open
        */
        Image_Resource_Disk_NITF( const std::filesystem::path& pathname );

        /**
         * Parameterized Constructor for writing images.
         * @param pathname Image to open
        */
        Image_Resource_Disk_NITF( const std::filesystem::path&             pathname,
                                  const Image_Format&                      output_format,
                                  const std::map<std::string,std::string>& write_options,
                                  const math::Size2i&                      block_size );

        /**
         * Destructs the resource
        */
        virtual ~Image_Resource_Disk_NITF() override = default;

        /**
         * Return the name of the resource
        */
        std::string resource_name() const override;

        /**
         * Create a new resource and open the image
         *
         * Result will return failure if it's unable to open the image.
        */
        static Result<ParentPtrT> create( const std::filesystem::path& pathname );

        /**
         * Create a new resource and open the image
         *
         * Result will return failure if it's unable to open the image.
        */
        static Result<ParentPtrT> create( const std::filesystem::path&             pathname,
                                          const Image_Format&                      output_format,
                                          const std::map<std::string,std::string>& write_options,
                                          const math::Size2i&                      block_size,
                                          const ColorCodeLookupT&                  color_reference_lut  = Use_Default_Color_LUT());

        /**
         * Open the dataset
        */
        Result<void> open( const std::filesystem::path& pathname );

        /**
         * Read the image data from disk
        */
        Result<void> read( const Image_Buffer& dest,
                           const math::Rect2i& bbox ) const override;

        /**
         * Write the resource to disk
        */
        Result<void> write( const Image_Buffer& dest,
                            const math::Rect2i& bbox ) override;

        /**
         * Get the image format object
        */
        Image_Format format() const override;

        /**
         * Block reads are not yet supported by NITF
        */
        bool has_block_read() const override;

        /**
         * Block writes are not yet supported by NITF
        */
        bool has_block_write() const override;

        /**
         * NITF does not support nodata reads yet.
        */
        bool has_nodata_read() const override;

        /**
         * NITF does not yet support nodata writes.
        */
        bool has_nodata_write() const override;

        /**
         * Get the block read size
        */
        math::Size2i  block_read_size() const override;

        /**
         * Get the block write size
        */
        math::Size2i block_write_size() const override;

        /**
         * Set the block write size
        */
        void set_block_write_size( const math::Size2i& block_size ) override;

        /**
         * Get the nodata read value
        */
        double nodata_read() const override;

        /**
         * Set the nodata write value
        */
        void set_nodata_write( double value ) override;

        /**
         * Flush the driver
        */
        void flush() override;

        /**
         * Print to log-friendly string
        */
        std::string to_log_string( size_t offset ) const override;

    private:


}; // End of Image_Resource_Disk_NITF class

} // end of tmns::image::io::nitf namespace