/**
 * @file    TEST_convert.cpp
 * @author  Marvin Smith
 * @date    10/6/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/pixel/convert.hpp>
#include <terminus/log/utility.hpp>

// C++ Libraries
#include <array>

/************************************/
/*      Convert Int to Float        */
/************************************/
TEST( image_convert, channel_convert_int_to_float )
{
    std::array<int8_t,4> int08_arr { 0, 50, 100, 127 };
    std::array<float,4>  flt32_arr { 0, 0, 0, 0 };

    std::array<float,4>  flt32_exp { 0, 0.393700, 0.787401, 1.0 };

    // Apply the operation.
    for( size_t i = 0; i < int08_arr.size(); i++ )
    {
        tmns::image::channel_convert_int_to_float( &int08_arr[i],
                                                   &flt32_arr[i] );

        ASSERT_NEAR( flt32_arr[i], flt32_exp[i], 0.001 );
    }

}

/************************************/
/*      Convert Float to Int        */
/************************************/
TEST( image_convert, channel_convert_float_to_int )
{
    std::array<float,4>   flt32_arr { 0.0f, 0.25f, 0.5f, 1.0f };
    std::array<uint8_t,4> uint08_arr { 0, 0, 0, 0 };

    std::array<uint8_t,4> uint08_exp { 0, 63, 127, 255 };

    // Apply the operation.
    for( size_t i = 0; i < flt32_arr.size(); i++ )
    {
        tmns::image::channel_convert_float_to_int( &flt32_arr[i],
                                                   &uint08_arr[i] );

        EXPECT_EQ( uint08_arr[i], uint08_exp[i] );
    }

}