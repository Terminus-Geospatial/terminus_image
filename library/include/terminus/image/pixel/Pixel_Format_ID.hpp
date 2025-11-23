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
 * @file    Pixel_Format_ID.hpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#pragma once

// Terminus Libraries
#include "Pixel_Format_Enum.hpp"
#include "Pixel_Gray.hpp"
#include "Pixel_GrayA.hpp"
#include "Pixel_RGB.hpp"
#include "Pixel_RGBA.hpp"

namespace tmns::image {

/**
 * Simple class for converting a formal Pixel-Type into a Pixel-Type-Enum
*/
template <typename PixelT>
struct Pixel_Format_ID
{
    static const Pixel_Format_Enum value = Pixel_Format_Enum::UNKNOWN;
}; // End of Pixel_Format_ID Class

/// Grayscale
template <typename ChannelT> struct Pixel_Format_ID<Pixel_Gray<ChannelT>>
{
    static const Pixel_Format_Enum value = Pixel_Format_Enum::GRAY;
}; // End of Grayscale

/// Grayscale w/ Alpha
template <typename ChannelT> struct Pixel_Format_ID<Pixel_GrayA<ChannelT>>
{
    static const Pixel_Format_Enum value = Pixel_Format_Enum::GRAYA;
}; // End of Grayscale/Alpha

/// RGB
template <typename ChannelT> struct Pixel_Format_ID<Pixel_RGB<ChannelT>>
{
    static const Pixel_Format_Enum value = Pixel_Format_Enum::RGB;
};

/// RGBA
template <typename ChannelT> struct Pixel_Format_ID<Pixel_RGBA<ChannelT>>
{
    static const Pixel_Format_Enum value = Pixel_Format_Enum::RGBA;
};

/// Scalar Types
template <> struct Pixel_Format_ID<int8_t>{   static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<uint8_t>{  static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<int16_t>{  static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<uint16_t>{ static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<int32_t>{  static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<uint32_t>{ static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<int64_t>{  static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<uint64_t>{ static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<float>{    static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };
template <> struct Pixel_Format_ID<double>{   static const Pixel_Format_Enum value = Pixel_Format_Enum::SCALAR; };

} // end of tmns::image namespace