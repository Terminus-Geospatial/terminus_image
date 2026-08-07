/**
 * @file    TEST_GDAL_Codes.cpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/io/drivers/gdal/GDAL_Codes.hpp>

/***************************************************/
/*   Test of the GDAL Color Codes                  */
/*                                                 */
/*   We need to make sure we don't get complacent  */
/*   and all of a sudden the values change in the  */
/*   GDAL API 10 years from now without us knowing.*/
/***************************************************/
TEST( GDAL_Codes, GDAL_Color_Codes )
{
    using namespace tmns::image::io::gdal;
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_Undefined,       0 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_GrayIndex,       1 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_PaletteIndex,    2 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_RedBand,         3 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_GreenBand,       4 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_BlueBand,        5 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_AlphaBand,       6 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_HueBand,         7 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_SaturationBand,  8 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_LightnessBand,   9 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_CyanBand,       10 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_MagentaBand,    11 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_YellowBand,     12 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_BlackBand,      13 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_YCbCr_YBand,    14 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_YCbCr_CbBand,   15 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_YCbCr_CrBand,   16 );
    ASSERT_EQ( (int)GDAL_Color_Codes::GCI_Max,            16 );
}