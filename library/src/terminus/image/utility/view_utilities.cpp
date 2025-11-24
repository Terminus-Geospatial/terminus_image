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
 * @file    View_Utilities.cpp
 * @author  Marvin Smith
 * @date    7/29/2023
*/
#include <terminus/image/utility/view_utilities.hpp>

// OpenCV Libraries
#include <opencv4/opencv2/highgui.hpp>

// Terminus Image Libraries
#include "opencv_utilities.hpp"

// Terminus Libraries
#include <terminus/log/utility.hpp>

namespace tmns::image::utility {

/********************************************/
/*          Actually Visualize Image        */
/********************************************/
Result<void> visualize( const std::string&  window_name,
                        const Image_Buffer& buffer_data,
                        int                 window_sleep )
{
    auto type_code = ocv::get_pixel_type_code( buffer_data.pixel_type(),
                                               buffer_data.channel_type() );
    if( type_code.has_error() )
    {
        return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                              "Unsupported conversion. ",
                              type_code.error().message() );
    }

    // convert to opencv image
    tmns::log::trace( "Creating OpenCV Image" );
    cv::Mat tmp_image( buffer_data.rows(),
                       buffer_data.cols(),
                       type_code.value(),
                       buffer_data.data() );

    tmns::log::trace( "Rendering window: ", window_name );
    cv::imshow( window_name.c_str(), tmp_image );
    cv::waitKey( window_sleep );
    return tmns::outcome::ok();
}

} // End of tmns::image::utility namespace