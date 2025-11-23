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
 * @file    Detector_Image_Utilities.hpp
 * @author  Marvin Smith
 * @date    9/2/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/log/Logger.hpp>

// Terminus Image Libraries
#include "../../../image/pixel/convert.hpp"

namespace tmns::feature::utility {

/**
 * Convert image to expected format needed by the detector.
 */
Result<image::Image_Buffer> prepare_image_buffer( const image::Image_Buffer& input_buffer,
                                                  bool                       cast_if_ctype_unsupported,
                                                  image::Pixel_Format_Enum   output_pixel_type,
                                                  image::Channel_Type_Enum   output_channel_type,
                                                  const std::string&         detector_name,
                                                  tmns::log::Logger&         logger,
                                                  std::mutex&                logger_mtx )
{
    // From testing, we know that ORB only likes integer images
    if( !cast_if_ctype_unsupported && input_buffer.channel_type() != output_channel_type )
    {
        return outcome::fail( core::error::ErrorCode::INVALID_CHANNEL_TYPE,
                              detector_name,
                              " module only support ", image::enum_to_string( output_channel_type ), 
                              " imagery.  You must enable casting.  Detected Channel-Type: ", 
                              image::enum_to_string( input_buffer.channel_type() ) );
    }

    // Check the number of channels, cast to single-channel if needed
    auto num_channels = image::num_channels( input_buffer.pixel_type() );
    if( num_channels.has_error() )
    {
        return outcome::fail( core::error::ErrorCode::INVALID_PIXEL_TYPE,
                              "Unable to determine input pixel type from buffer.",
                              " Detected Type: ", image::enum_to_string( input_buffer.pixel_type() ) );
    }

    // Temporary structure for casting data
    image::Image_Buffer detect_buffer = input_buffer;
    const bool DO_RESCALE { true };
    std::vector<uint8_t> temp_image_data;
    bool perform_cast = false;
    image::Image_Format new_format = input_buffer.format();

    // Update dest pixel type if not grayscal
    if( num_channels.value() != 1 )
    {
        new_format.set_pixel_type( output_pixel_type );
        perform_cast = true;
    }

    // Update channel type if not uint8
    if( input_buffer.channel_type() != output_channel_type )
    {
        new_format.set_channel_type( output_channel_type );
        perform_cast = true;
    }

    // Execute the conversion
    if( perform_cast )
    {
        // Create temporary storage for pixel data
        temp_image_data.resize( new_format.raster_size_bytes() );
        detect_buffer = image::Image_Buffer( new_format,
                                             temp_image_data.data() );

        {
            std::unique_lock<std::mutex> lck( logger_mtx );
            logger.trace( "Casting image buffer data. \nInput: ", 
                          input_buffer.to_string(), 
                          "\nOutput: ", 
                          detect_buffer.to_string() );
        }
        auto cast_result = image::convert( detect_buffer,
                                           input_buffer,
                                           DO_RESCALE );
        if( cast_result.has_error() )
        {
            return outcome::fail( core::error::ErrorCode::CONVERSION_ERROR,
                                  "Unable to convert image-buffer to grayscale for processing." );
        }
    }
    else
    {
        std::unique_lock<std::mutex> lck( logger_mtx );
        logger.trace( "No need to cast buffer data. ", 
                      detect_buffer.to_string() );
    }

    return outcome::ok<image::Image_Buffer>( detect_buffer );
}


} // End of tmns::feature::utility