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
 * @file    Pixel_Format_Enum.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

namespace tmns::image {

/**
 * Structure representing pixel type enumerations and names
*/
enum class Pixel_Format_Enum
{
    UNKNOWN = 0,
    SCALAR  = 1,
    GRAY    = 2,
    GRAYA   = 3,
    RGB     = 4,
    RGBA    = 5,
    HSV     = 6,
    XYZ     = 7,
    LUV     = 8,
    LAB     = 9,
    UNKNOWN_MASKED    = 20,
    SCALAR_MASKED     = 21,
    GRAY_MASKED       = 22,
    GRAYA_MASKED      = 23,
    RGB_MASKED        = 24,
    RGBA_MASKED       = 25,
    HSV_MASKED        = 26,
    XYZ_MASKED        = 27,
    LUV_MASKED        = 28,
    LAB_MASKED        = 29,
    GENERIC_1_CHANNEL = 60,
    GENERIC_2_CHANNEL = 61,
    GENERIC_3_CHANNEL = 62,
    GENERIC_4_CHANNEL = 63,
    GENERIC_5_CHANNEL = 64,
    GENERIC_6_CHANNEL = 65,
    GENERIC_7_CHANNEL = 66,
    GENERIC_8_CHANNEL = 67,
    GENERIC_9_CHANNEL = 68,
}; // End of Pixel_Format_Enum enumeration

/**
 * Convert enumeration to string
*/
std::string enum_to_string( Pixel_Format_Enum val );

/**
 * Get the number of channels for the given pixel format enumeration.
*/
Result<int> num_channels( Pixel_Format_Enum value );

} // End of tmns::image namespace