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
 * @file    Driver_Factory_Base.hpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#pragma once

// C++ Libraries
#include <filesystem>
#include <memory>

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

// Terminus Image Libraries
#include "../../types/Image_Format.hpp"
#include "../../types/Image_Resource_Base.hpp"

namespace tmns::image::io{

/**
 * @class Driver_Factory_Base
 *
 * Simple class to help construct Read/Write image resources
*/
class Driver_Factory_Base
{
    public:

        /// Disk Driver Pointer Type
        typedef Image_Resource_Base::ptr_t DriverT;

        /// Pointer Type
        typedef std::shared_ptr<Driver_Factory_Base> ptr_t;

        /// Destructor
        virtual ~Driver_Factory_Base() = default;

        /**
         * Check if the file type is supported for this driver
        */
        virtual bool is_read_image_supported( const std::filesystem::path& pathname ) const = 0;

        /**
         * Check if the file type is supported for this driver.
        */
        virtual bool is_write_image_supported( const std::filesystem::path& pathname ) const = 0;

        /**
         * Build a new Read-Image-Resource object.
        */
        virtual Result<DriverT> create_read_driver( const std::filesystem::path& pathname ) const = 0;

        /**
         * Create a new Write-Image-Resource object.
         *
         * @param desired_planes Split image into single image per plane. -1 to not set and use input image settings.
        */
        virtual Result<DriverT> create_write_driver( const std::filesystem::path&              pathname,
                                                     const Image_Format&                       output_format,
                                                     const std::map<std::string,std::string>&  write_options,
                                                     const math::Size2i&                       block_size ) const = 0;

}; // end of Driver_Factory_Base class

} // End of tmns::image::io namespace