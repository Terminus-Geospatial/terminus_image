/**
 * @file    TEST_OpenCV_Utilities.cpp
 * @author  Marvin Smith
 * @date    8/11/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include "../../../../src/terminus/image/utility/OpenCV_Utilities.hpp"

namespace tx = tmns::image;

/************************************************/
/*      Test OpenCV Type to String Method       */
/************************************************/
TEST( OpenCV_Utilities, opencv_type_to_string )
{
    ASSERT_EQ( tx::utility::ocv::opencv_type_to_string( CV_8UC1 ), "CV_8UC1" );
    ASSERT_EQ( tx::utility::ocv::opencv_type_to_string( CV_8UC1 ), "CV_16UC1" );
}