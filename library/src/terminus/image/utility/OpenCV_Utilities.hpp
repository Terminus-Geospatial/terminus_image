/**
 * @file    OpenCV_Utilities.hpp
 * @author  Marvin Smith
 * @date    7/29/2023
*/
#pragma once

// Terminus Image Libraries
#include <terminus/image/pixel/Channel_Type_Enum.hpp>
#include <terminus/image/pixel/Pixel_Format_Enum.hpp>

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>


namespace tmns::image::utility::ocv {

/**
 * Given the channel type, generate an OpenCV channel code (ex: CV_8U)
*/
Result<int> get_data_type_code( Channel_Type_Enum channel_type );

/**
 * Given an pixel-type and channel-type, generate the pixel type
 * code required to create an OpenCV Mat.
*/
Result<int> get_pixel_type_code( Pixel_Format_Enum  pixel_type,
                                 Channel_Type_Enum  channel_type );

/**
 * Convert the OpenCV type code to a string
*/
std::string opencv_type_to_string( int tp );

} // End of tmns::image::utility::ocv