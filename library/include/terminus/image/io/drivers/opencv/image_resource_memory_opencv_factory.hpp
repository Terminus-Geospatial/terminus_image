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
 * @file    Image_Resource_Memory_OpenCV_Factory.hpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#pragma once

/// Terminus Libraries
#include "../Driver_Factory_Base.hpp"
#include "Image_Resource_Memory_OpenCV.hpp"

namespace tmns::image::io::ocv {

/**
 * Exists purely to crank out OpenCV Read/Write Resources.
*/
class Image_Resource_Memory_OpenCV_Factory : public Driver_Factory_Base
{
    public:

        /// Disk Driver Pointer Type
        typedef Image_Resource_Base::ptr_t DriverT;

        Image_Resource_Memory_OpenCV_Factory() = default;

        virtual ~Image_Resource_Memory_OpenCV_Factory() override = default;

        /**
         * Check if image type is supported by the GDAL driver
        */
        bool is_read_image_supported( const std::filesystem::path& pathname ) const override;

        /**
         * Check if the image type is supported by GDAL for write operations
        */
        bool is_write_image_supported( const std::filesystem::path& pathname ) const override;

        /**
         * Create the OpenCV Image Memory-Reader Resource
        */
        Result<DriverT> create_read_driver( const std::filesystem::path& pathname ) const override;

        /**
         * Create the OpenCV Image Memory-Write Resource
        */
        Result<DriverT> create_write_driver( const std::filesystem::path&             pathname,
                                             const Image_Format&                      output_format,
                                             const std::map<std::string,std::string>& write_options,
                                             const math::Size2i&                      block_size ) const override;

    private:

        /// List of supported extensions
        std::vector<std::string> m_supported_extensions { ".tif", ".png", ".jpg" };

}; // end of Image_Resource_Memory_OpenCV_Factory

} // end of tmns::image::io::ocv namespace