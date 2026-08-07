/**
 * @file    TEST_GDAL_Utilities.cpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#include <gtest/gtest.h>

// GDAL Libraries
#include <gdal.h>

// Terminus Libraries
#include <terminus/image/io/drivers/gdal/GDAL_Utilities.hpp>
#include <terminus/image/io/drivers/gdal/Image_Resource_Disk_GDAL.hpp>

using namespace tmns::image;

/********************************************************/
/*          Test the driver to pixel type method        */
/********************************************************/
TEST( GDAL_Utilities, gdal_driver_to_pixel_type )
{
    // I'm using this to just make sure my logic is sort of on the mark
    auto reference_lut_01 = io::gdal::Image_Resource_Disk_GDAL::Use_Default_Color_LUT();

    // Verify grayscale
    std::vector<int> test_entry_gray{ GCI_GrayIndex };
    auto result = io::gdal::gdal_driver_to_pixel_type( reference_lut_01,
                                                       test_entry_gray );
    ASSERT_FALSE( result.has_error() );
    ASSERT_EQ( result.value(), Pixel_Format_Enum::GRAY );

    std::vector<int> a { 1, 2 };
    std::vector<int> b { 1, 2, 2 };
    ASSERT_FALSE( a == b );
    // Verify grayscale w/ alpha
    std::vector<int> test_entry_gray_alpha{ GCI_GrayIndex,
                                            GCI_AlphaBand };
    auto result2 = io::gdal::gdal_driver_to_pixel_type( reference_lut_01,
                                                        test_entry_gray_alpha );
    ASSERT_FALSE( result2.has_error() );
    ASSERT_EQ( result2.value(), Pixel_Format_Enum::GRAYA );

    // Verify rgb
    std::vector<int> test_entry_rgb{ GCI_RedBand,
                                     GCI_GreenBand,
                                     GCI_BlueBand };
    result = io::gdal::gdal_driver_to_pixel_type( reference_lut_01,
                                                   test_entry_rgb );
    ASSERT_FALSE( result.has_error() );
    ASSERT_EQ( result.value(), Pixel_Format_Enum::RGB );

    // Verify rgb w/ alpha
    std::vector<int> test_entry_rgba{ GCI_RedBand,
                                      GCI_GreenBand,
                                      GCI_BlueBand,
                                      GCI_AlphaBand };
    result = io::gdal::gdal_driver_to_pixel_type( reference_lut_01,
                                                  test_entry_rgba );
    ASSERT_FALSE( result.has_error() );
    ASSERT_EQ( result.value(), Pixel_Format_Enum::RGBA );
}

/********************************************/
/*          Test the Pixel-Format           */
/*         to Channel-Type-ID Method        */
/********************************************/
TEST( GDAL_Utilities, gdal_pixel_format_to_channel_type )
{
    /// Uint8
    auto result = io::gdal::gdal_pixel_format_to_channel_type( GDT_Byte );
    ASSERT_FALSE( result.has_error() );
    ASSERT_EQ( result.value(), Channel_Type_Enum::UINT8 );

    /// Uint16
    result = io::gdal::gdal_pixel_format_to_channel_type( GDT_UInt16 );
    ASSERT_FALSE( result.has_error() );
    ASSERT_EQ( result.value(), Channel_Type_Enum::UINT16 );
}