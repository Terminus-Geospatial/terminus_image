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
 * @file    read_image.hpp
 * @author  Marvin Smith
 * @date    7/10/2023
*/
#pragma once

// External Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/log/utility.hpp>
#include <terminus/outcome/Result.hpp>

// Terminus Libraries
#include "../types/Image_Memory.hpp"
#include "drivers/Disk_Driver_Manager.hpp"
#include "drivers/Memory_Driver_Manager.hpp"
#include "Image_Resource_Disk.hpp"

// C++ Libraries
#include <filesystem>

namespace tmns::image::io {

/**
 * Load an image from a pre-initialized resource.  Also requires a bounding box to allow for
 * cropping the input image.
*/
template <typename PixelT>
Result<Image_Memory<PixelT>> read_image_from_resource( const Image_Resource_Disk::ptr_t resource,
                                                       const math::Rect2i&              bbox )
{
    // Create output image
    Image_Memory<PixelT> output_image;
    int planes = 1;
    auto size_res = output_image.set_size( bbox.width(),
                                           bbox.height(),
                                           planes );
    if( size_res.has_error() )
    {
        return outcome::fail( size_res.error() );
    }

    // Pass the image data
    auto load_res = resource->read( output_image.buffer(),
                                    bbox );

    if( load_res.has_error() )
    {
        return outcome::fail( load_res.error() );
    }
    else
    {
        return outcome::ok<Image_Memory<PixelT>>( std::move( output_image ) );
    }
}

/**
 * Load an image from an image-resource.
 * @param resource Pre-configured image resource.
 *
 * Note:  The primary method requires a bounding box, so the full image bounds will be used for this method.
*/
template <typename PixelT>
Result<Image_Memory<PixelT>> read_image_from_resource( const Image_Resource_Disk::ptr_t resource )
{
    return read_image_from_resource<PixelT>( resource,
                                             math::Rect2i( 0, 0,
                                                           resource->cols(),
                                                           resource->rows() ) );
}

/**
 * Load an image from an image-resource into an Image_Memory container
 * This differs from previous methods as the image is provided as an input parameter and must stay constructed.
*/
template <class PixelT>
Result<void> read_image( const Image_Memory<PixelT>&           dst,
                         const Read_Image_Resource_Base::ptr_t src,
                         const math::Rect2i&                   bbox )
{
    src->read( dst.buffer(), bbox );
    return outcome::ok();
}

/**
 * Load an image into a generic image type container
*/
template <typename ImageT>
Result<void> read_image( const Image_Base<ImageT>&             dest,
                         const Read_Image_Resource_Base::ptr_t src,
                         const math::Rect2i&                   bbox )
{
    Image_Memory<typename ImageT::pixel_type> intermediate;
    read_image( intermediate, src, bbox );
    dest.impl() = intermediate;
    return outcome::ok();
}

/**
 * Load an image from disk
 *
 * @param[in] pathname Image to load
 * @param[out] image Image after loading.
 *
 * @return Status of the operation
 *
 * Note the image has to be returned as we are using
*/
template <typename PixelT>
Result<Image_Memory<PixelT>> read_image( const std::filesystem::path&      pathname,
                                         const Disk_Driver_Manager::ptr_t  driver_manager = Disk_Driver_Manager::create_read_defaults() )
{
    tmns::log::info( "Loading image: ", pathname.string() );

    // Create an image resource for the data
    auto driver_res = driver_manager->pick_read_driver( pathname );
    if( driver_res.has_error() )
    {
        return outcome::fail( driver_res.assume_error() );
    }
    auto image_resource = driver_res.assume_value();

    auto read_result = read_image_from_resource<PixelT>( image_resource );
    if( read_result.has_error() )
    {
        return read_result.error();
    }
    return outcome::ok<Image_Memory<PixelT>>( std::move( read_result.value() ) );
}

} // End of tmns::image::io namespace