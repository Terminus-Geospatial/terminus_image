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
 * @file    read_image_disk.hpp
 * @author  Marvin Smith
 * @date    7/28/2023
*/
#pragma once

// External Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/log/utility.hpp>
#include <terminus/outcome/Result.hpp>

// Terminus Libraries
#include "../types/Image_Disk.hpp"
#include "read_image.hpp"

// C++ Libraries
#include <filesystem>

namespace tmns::image::io {


/**
 * Load an image from disk into an Image_Disk object, rather than memory
 *
 * @param pathname Path of image to load from disk.
 * @param driver_manager Factory for creating resources.  Allows you to inject your own drivers without touching
 *                       too deep into the guts of Terminus.
 *
 * @return Instance of image.  Note that a Disk-Image is lazy and doesn't actually pull it into ram.  Calls to `rasterize()`
 *          will be painful.
*/
template <typename PixelT>
Result<Image_Disk<PixelT>> read_image_disk( const std::filesystem::path&      pathname,
                                            const Disk_Driver_Manager::ptr_t  driver_manager = Disk_Driver_Manager::create_read_defaults(),
                                            core::cache::Cache_Local::ptr_t   cache = std::make_shared<core::cache::Cache_Local>( 1000000000 ) )
{
    // Create an image resource for the data
    auto driver_res = driver_manager->pick_read_driver( pathname );
    if( driver_res.has_error() )
    {
        return outcome::fail( driver_res.error() );
    }
    auto image_resource = driver_res.assume_value();

    Image_Disk<PixelT> image( image_resource,
                              cache );
    return outcome::ok<Image_Disk<PixelT>>( std::move( image ) );
}

} // End of tmns::image::io namespace