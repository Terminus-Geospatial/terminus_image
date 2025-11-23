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
 * @file    Image_Resource_Disk_GDAL.hpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#pragma once

// Terminus Libraries
#include "../../Image_Resource_Disk.hpp"
#include "GDAL_Codes.hpp"

// C++ Libraries
#include <tuple>
#include <vector>

namespace tmns::image::io::gdal {

class GDAL_Disk_Image_Impl;

/**
 * @class Image_Resource_Disk_GDAL
 *
 * Disk Read/Write Interface for GDAL.
*/
class Image_Resource_Disk_GDAL : public Image_Resource_Disk
{
    public:

        /// Parent Pointer Type
        typedef Image_Resource_Disk::ptr_t ParentPtrT;

        /// Lookup Table for mapping GDAL Code Codes to Terminus Pixel-Types
        typedef std::vector<std::tuple<std::vector<int>,Pixel_Format_Enum>> ColorCodeLookupT;

        /**
         * Parameterized Constructor for reading images.
         * @param pathname Image to open
        */
        Image_Resource_Disk_GDAL( const std::filesystem::path& pathname,
                                  ColorCodeLookupT             color_reference_lut = Use_Default_Color_LUT() );

        /**
         * Parameterized Constructor for writing images.
         * @param pathname Image to open
        */
        Image_Resource_Disk_GDAL( const std::filesystem::path&             pathname,
                                  const Image_Format&                      output_format,
                                  const std::map<std::string,std::string>& write_options,
                                  const math::Size2i&                      block_size,
                                  const ColorCodeLookupT&                  color_reference_lut = Use_Default_Color_LUT() );

        /**
         * Destructs the resource in 3 steps
         * 1. Flushes all data in the resource
         * 2. Acquires the global GDAL resource mutex lock
         * 3. Deletes all GDAL references and cleans up.
        */
        virtual ~Image_Resource_Disk_GDAL() override;

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
         * Block reads are supported by GDAL
        */
        bool has_block_read() const override;

        /**
         * Block writes are supported by GDAL
        */
        bool has_block_write() const override;

        /**
         * GDAL generally supports nodata reads as long as the format you
         * are loading also supports it.
        */
        bool has_nodata_read() const override;

        /**
         * GDAL Supports nodata writes
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

        /**
         * Check if GDAL supports the filename.
        */
        static bool gdal_has_support( const std::string& filename );

        /**
         * Default mapping of Color-Codes in GDAL to Pixel-Types
        */
        inline static ColorCodeLookupT Use_Default_Color_LUT()
        {
            ColorCodeLookupT lut;

            // Grayscale
            lut.push_back( std::make_tuple( std::vector<int>{ static_cast<int>(GDAL_Color_Codes::GCI_GrayIndex) },
                                            Pixel_Format_Enum::GRAY ) );

            // Grayscale with alpha
            lut.push_back( std::make_tuple( std::vector<int>{ static_cast<int>(GDAL_Color_Codes::GCI_GrayIndex),
                                                              static_cast<int>(GDAL_Color_Codes::GCI_AlphaBand) },
                                            Pixel_Format_Enum::GRAYA ) );

            // RGB
            lut.push_back( std::make_tuple( std::vector<int>{ static_cast<int>(GDAL_Color_Codes::GCI_RedBand),
                                                              static_cast<int>(GDAL_Color_Codes::GCI_GreenBand),
                                                              static_cast<int>(GDAL_Color_Codes::GCI_BlueBand) },
                                            Pixel_Format_Enum::RGB ) );

            // RGBA
            lut.push_back( std::make_tuple( std::vector<int>{ static_cast<int>(GDAL_Color_Codes::GCI_RedBand),
                                                              static_cast<int>(GDAL_Color_Codes::GCI_GreenBand),
                                                              static_cast<int>(GDAL_Color_Codes::GCI_BlueBand),
                                                              static_cast<int>(GDAL_Color_Codes::GCI_AlphaBand) },
                                            Pixel_Format_Enum::RGBA ) );

            return lut;
        }

    private:

        std::shared_ptr<GDAL_Disk_Image_Impl> m_impl;

        /// Color Code Lookup Table
        ColorCodeLookupT m_color_reference_lut;

}; // End of Image_Resource_Disk_GDAL class

} // end of tmns::image::io::gdal namespace