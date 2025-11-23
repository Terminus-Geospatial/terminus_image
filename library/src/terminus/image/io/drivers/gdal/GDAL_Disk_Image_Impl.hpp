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
 * @file    GDAL_Disk_Image_Impl.hpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#pragma once

// GDAL Libraries
#include <gdal.h>
#include <gdal_priv.h>

// C++ Libraries
#include <filesystem>
#include <map>
#include <tuple>
#include <vector>

// External Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/log/Logger.hpp>
#include <terminus/math/Rectangle.hpp>
#include <terminus/math/Size.hpp>
#include <terminus/outcome/Result.hpp>

// Terminus Libraries
#include "../../../metadata/Metadata_Container_Base.hpp"
#include "../../../pixel/Pixel_Format_Enum.hpp"
#include "../../../pixel/Pixel_RGBA.hpp"
#include "../../../types/Image_Buffer.hpp"
#include "../../../types/Image_Format.hpp"

namespace tmns::image::io::gdal {

class GDAL_Disk_Image_Impl
{
    public:

        typedef std::map<std::string,std::string> Options;

        typedef std::shared_ptr<GDALDataset> DatasetPtrT;

        typedef std::vector<std::tuple<std::vector<int>,Pixel_Format_Enum>> ColorCodeLookupT;

        /**
         * Constructor for when you want to read data.
        */
        GDAL_Disk_Image_Impl( const std::filesystem::path& pathname,
                              const ColorCodeLookupT&      color_reference_lut );

        /**
         * Constructor for when you want to write data.
        */
        GDAL_Disk_Image_Impl( const std::filesystem::path&             pathname,
                              const Image_Format&                      output_format,
                              const std::map<std::string,std::string>& write_options,
                              const math::Size2i&                      block_size,
                              const ColorCodeLookupT&                  color_reference_lut );

        /**
         * Open the dataset
        */
        Result<void> open( const std::filesystem::path& pathname );

        /**
         * Read the raster from disk
        */
        Result<void> read( const Image_Buffer&  dest,
                           const math::Rect2i&  bbox,
                           bool                 rescale ) const;

        /**
         * Write the resource to disk
        */
        Result<void> write( const Image_Buffer& dest,
                            const math::Rect2i& bbox,
                            bool                rescale );

        /**
         * Print to log-friendly string
        */
        std::string To_Log_String( size_t offset ) const;

        /**
         * Get the image format
        */
        Image_Format format() const;

        /**
         * Get the GDALDataset point for whatever dataset is active.
        */
        Result<DatasetPtrT> get_dataset_ptr() const;

        /**
         * Get the default block size
        */
        math::Size2i default_block_size() const;

        /**
         * Check if driver has nodata read
        */
        bool has_nodata_read() const;

        /**
         * Get the block read size
        */
        math::Size2i block_read_size() const;

        /**
         * Get the block write size
        */
        math::Size2i block_write_size() const;

        /**
         * Set the block write size
        */
        void set_block_write_size( const math::Size2i& block_size );

        /**
         * Get the nodata read value
        */
        double nodata_read() const;

        /**
         * Set the nodata write value
        */
        void set_nodata_write( double value );

        /**
         * Flush the image
        */
        void flush();

        /**
         * Get the internal metadata container
         */
        meta::Metadata_Container_Base::ptr_t metadata() const;

        /**
         * Check if driver type is trusted to report valid single-line block sizes
        */
        static bool blocksize_whitelist( const GDALDriver* driver );

        /**
         * Check if GDAL supports the filename
         *
         * Lazy wrapper around GDAL's @todo call
        */
        static bool gdal_has_support( const std::string& filename );

    private:

        void  initialize_write_resource_locked();

        /**
         * Check the driver to see if the nodata read value was acceptable
        */
        Result<double> nodata_read_ok() const;

        /**
         * Setup the GDAL structure for writing files
        */
        void configure_for_writing( const Image_Format&                      output_format,
                                    const std::map<std::string,std::string>& write_options,
                                    const math::Size2i&                      block_size );

        /**
         * Process Dataset Metadata
         */
        void process_metadata( log::Logger&                 logger, 
                               std::shared_ptr<GDALDataset> dataset );

        /// Pathname to image
        std::filesystem::path m_pathname;

        /// GDAL Datasets
        std::shared_ptr<GDALDataset> m_read_dataset;
        std::shared_ptr<GDALDataset> m_write_dataset;

        /// Format Information
        Image_Format m_format;

        /// Color Code -> PixelType lookup table
        ColorCodeLookupT m_color_reference_lut;

        /// Block Size Information
        math::Size2i m_blocksize;

        /// Image Palette
        std::vector<PixelRGBA_u8> m_color_table;

        // Base Driver Options
        Options  m_driver_options;

        /// Metadata Container
        meta::Metadata_Container_Base::ptr_t m_metadata { std::make_shared<meta::Metadata_Container_Base>() };

}; // End of GDAL_Disk_Image_Impl class

}