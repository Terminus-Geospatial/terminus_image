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
 * @file    GDAL_Codes.hpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#pragma once

// C++ Libraries
#include <string>
#include <vector>

/**
 * This entire header is somewhat superflerous as we're simply remapping GDAL enums.  The
 * only reason this exists is because we really, really, really don't want the consumer
 * to be required to #include<gdal.h> when using our products, so we're wrapping GDAL behind
 * the PIMPL idiom.
 *
 * We have an internal unit-test that verifies these values don't change.
*/
namespace tmns::image::io::gdal {

// This is directly copied from `gdal.h`
enum class GDAL_Color_Codes
{
    /*! Undefined */                             GCI_Undefined      = 0,
    /*! Greyscale */                             GCI_GrayIndex      = 1,
    /*! Paletted (see associated color table) */ GCI_PaletteIndex   = 2,
    /*! Red band of RGBA image */                GCI_RedBand        = 3,
    /*! Green band of RGBA image */              GCI_GreenBand      = 4,
    /*! Blue band of RGBA image */               GCI_BlueBand       = 5,
    /*! Alpha (0=transparent, 255=opaque) */     GCI_AlphaBand      = 6,
    /*! Hue band of HLS image */                 GCI_HueBand        = 7,
    /*! Saturation band of HLS image */          GCI_SaturationBand = 8,
    /*! Lightness band of HLS image */           GCI_LightnessBand  = 9,
    /*! Cyan band of CMYK image */               GCI_CyanBand       = 10,
    /*! Magenta band of CMYK image */            GCI_MagentaBand    = 11,
    /*! Yellow band of CMYK image */             GCI_YellowBand     = 12,
    /*! Black band of CMYK image */              GCI_BlackBand      = 13,
    /*! Y Luminance */                           GCI_YCbCr_YBand    = 14,
    /*! Cb Chroma */                             GCI_YCbCr_CbBand   = 15,
    /*! Cr Chroma */                             GCI_YCbCr_CrBand   = 16,
    /*! Max current value (equals to GCI_YCbCr_CrBand currently) */ GCI_Max = 16
}; // end of GDAL_Color_Codes Enum

/**
 * Create a log-friendly string showing the color codes
*/
std::string ToLogString( std::vector<int> codes );

} // end of tmns::image::io::gdal namespace