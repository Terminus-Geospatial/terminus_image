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
 * @file    detector_ocv_gftt.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include <terminus/feature/drivers/ocv/detector_ocv_gftt.hpp>

// Terminus Libraries
#include <terminus/error.hpp>

// Terminus Image Libraries
#include <terminus/image/pixel/convert.hpp>
#include <terminus/image/utility/opencv_utilities.hpp>

// OpenCV Libraries
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

namespace tmns::feature::ocv {

/************************************/
/*      Default Constructor         */
/************************************/
detector_OCV_GFTT::detector_OCV_GFTT()
    : detector_OCV_Base( std::make_shared<detector_Config_OCV_GFTT>() ),
      m_config( std::dynamic_pointer_cast<detector_Config_OCV_GFTT>( this->get_ocv_detector_config() ) )
{
}

/****************************************/
/*      Parameterized Constructor       */
/****************************************/
detector_OCV_GFTT::detector_OCV_GFTT( const detector_Config_Base::ptr_t config )
    : detector_OCV_Base( config ),
      m_config( std::dynamic_pointer_cast<detector_Config_OCV_GFTT>( config ) )
{
}

/**********************************/
/*    Run tracker on image data   */
/**********************************/
Result<interest_point_List> detector_OCV_GFTT::process_image( const image::Image_Buffer& buffer,
                                                              bool                       cast_if_ctype_unsupported,
                                                              int                        max_points_override )
{
    // From testing, we know that GFTT only likes integer images
    if( !cast_if_ctype_unsupported && buffer.channel_type() != image::Channel_Type_Enum::UINT8 )
    {
        return outcome::fail( error::Error_Code::INVALID_CHANNEL_TYPE,
                              class_name(),
                              " module only support uint8 imagery.  You must enable casting.",
                              ", Detected Channel-Type: ", image::enum_to_string( buffer.channel_type() ) );
    }

    // Check the number of channels, cast to single-channel if needed
    auto num_channels = image::num_channels( buffer.pixel_type() );
    if( num_channels.has_error() )
    {
        return outcome::fail( error::Error_Code::INVALID_PIXEL_TYPE,
                              "Unable to determine input pixel type from buffer.",
                              " Detected Type: ", image::enum_to_string( buffer.pixel_type() ) );
    }

    // Temporary structure for casting data
    image::Image_Buffer detect_buffer = buffer;
    const bool DO_RESCALE { true };
    std::vector<uint8_t> temp_image_data;
    bool perform_cast = false;
    image::Image_Format new_format = buffer.format();

    // Update dest pixel type if not grayscal
    if( num_channels.value() != 1 )
    {
        new_format.set_pixel_type( image::Pixel_Format_Enum::GRAY );
        perform_cast = true;
    }

    // Update channel type if not uint8
    if( buffer.channel_type() != image::Channel_Type_Enum::UINT8 )
    {
        new_format.set_channel_type( image::Channel_Type_Enum::UINT8 );
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
            std::unique_lock<std::mutex> lck( m_log_mtx );
            m_logger.trace( "Casting image buffer data. \nInput: ",
                            buffer.to_string(),
                            "\nOutput: ",
                            detect_buffer.to_string() );
        }
        auto cast_result = image::convert( detect_buffer,
                                           buffer,
                                           DO_RESCALE );
        if( cast_result.has_error() )
        {
            return outcome::fail( error::Error_Code::CONVERSION_ERROR,
                                  "Unable to convert image-buffer to grayscale for processing." );
        }
    }
    else
    {
        std::unique_lock<std::mutex> lck( m_log_mtx );
        m_logger.trace( "No need to cast buffer data. ",
                        detect_buffer.to_string() );
    }

    // Create the opencv image to run detection on
    auto type_code = image::utility::ocv::get_pixel_type_code( detect_buffer.pixel_type(),
                                                               detect_buffer.channel_type() );
    if( type_code.has_error() )
    {
        return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                              "Unsupported conversion. ",
                              type_code.error().message() );
    }

    cv::Mat image( detect_buffer.rows(),
                   detect_buffer.cols(),
                   type_code.value(),
                   detect_buffer.data() );
    tmns::log::info( ADD_CURRENT_LOC(), image::utility::ocv::opencv_type_to_string( type_code.value() ) );

    // Build the feature detector
    int max_points = ( max_points_override > 0 ) ? max_points_override : m_config->max_features();
    auto detector = cv::GFTTDetector::create( max_points,
                                              m_config->quality_level(),
                                              m_config->min_distance(),
                                              m_config->block_size(),
                                              m_config->use_harris_detector(),
                                              m_config->k() );


    // Run detect
    tmns::log::trace( "Calling detect" );
    std::vector<cv::KeyPoint> kps;
    detector->detect( image, kps );
    tmns::log::trace( "Located ", kps.size(), " keypoints" );

    interest_point_List points( kps.size() );
    for( size_t i = 0; i < kps.size(); i++ )
    {
        points.emplace_back( kps[i].pt.x, kps[i].pt.y );
    }
    return outcome::ok<interest_point_List>( points );
}

/****************************/
/*    Get the class name    */
/****************************/
std::string detector_OCV_GFTT::class_name() const
{
    return "detector_OCV_GFTT";
}

/************************************/
/*      Generate New Instance       */
/************************************/
Result<detector_Base::ptr_t> detector_Generator_OCV_GFTT::generate( detector_Config_Base::ptr_t config )
{
    // Check if the detector config GFTT
    bool same = ( dynamic_cast<detector_Config_OCV_GFTT*>( config.get() ) != nullptr );
    if( same )
    {
        return outcome::ok<detector_Base::ptr_t>( std::make_shared<detector_OCV_GFTT>( config ) );
    }
    else
    {
        return outcome::fail( error::Error_Code::NOT_FOUND,
                              "Config doesn't match" );
    }
}

} // End of tmns::feature::ocv namespace