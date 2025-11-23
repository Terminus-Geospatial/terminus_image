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
 * @file    Image_Resource_Disk_NITF_Factory.hpp
 * @author  Marvin Smith
 * @date    7/19/2024
*/
#pragma once

/// Terminus Libraries
#include "../Driver_Factory_Base.hpp"
#include "Image_Resource_Disk_NITF.hpp"

namespace tmns::image::io::nitf {

/**
 * Exists purely to crank out NITF Read/Write Resources.
*/
class Image_Resource_Disk_NITF_Factory : public Driver_Factory_Base
{
    public:

        /// Disk Driver Pointer Type
        typedef Image_Resource_Base::ptr_t DriverT;

        Image_Resource_Disk_NITF_Factory() = default;

        virtual ~Image_Resource_Disk_NITF_Factory() override = default;

        /**
         * Check if image type is supported by the NITF driver
        */
        bool is_read_image_supported( const std::filesystem::path& pathname ) const override;

        /**
         * Check if the image type is supported by NITF for write operations
        */
        bool is_write_image_supported( const std::filesystem::path& pathname ) const override;

        /**
         * Create the NITF Image Disk-Reader Resource
        */
        Result<DriverT> create_read_driver( const std::filesystem::path& pathname ) const override;

        /**
         * Create the NITF Image Disk-Write Resource
        */
        Result<DriverT> create_write_driver( const std::filesystem::path&             pathname,
                                             const Image_Format&                      output_format,
                                             const std::map<std::string,std::string>& write_options,
                                             const math::Size2i&                      block_size ) const override;

    private:

        /// List of supported extensions
        std::vector<std::string> m_supported_extensions { ".ntf", ".nitf" };

}; // end of Image_Resource_Disk_NITF_Factory

} // end of tmns::image::io::nitf namespace