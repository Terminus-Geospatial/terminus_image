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
 * @file    Detector_OCV_ORB.cpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#include "Detector_OCV_ORB.hpp"

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

// Terminus Image Libraries
#include "../../utility/Detector_Image_Utilities.hpp"
#include "../../../image/utility/OpenCV_Utilities.hpp"

// OpenCV Libraries
#include <opencv2/core/types.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

namespace tmns::feature::ocv {

/************************************/
/*      Default Constructor         */
/************************************/
Detector_OCV_ORB::Detector_OCV_ORB()
    : Detector_OCV_Base( std::make_shared<Detector_Config_OCV_ORB>() ),
      m_config( std::dynamic_pointer_cast<Detector_Config_OCV_ORB>( this->get_ocv_detector_config() ) )
{
}

/****************************************/
/*      Parameterized Constructor       */
/****************************************/
Detector_OCV_ORB::Detector_OCV_ORB( const Detector_Config_Base::ptr_t config )
    : Detector_OCV_Base( config ),
      m_config( std::dynamic_pointer_cast<Detector_Config_OCV_ORB>( config ) )
{
}

/**********************************/
/*    Run tracker on image data   */
/**********************************/
Result<Interest_Point_List> Detector_OCV_ORB::process_image( const image::Image_Buffer& buffer,
                                                             bool                       cast_if_ctype_unsupported,
                                                             int                        max_points_override = 0 )
{
    // Process the image
    auto proc_res = utility::prepare_image_buffer( buffer,
                                                   cast_if_ctype_unsupported,
                                                   image::Pixel_Format_Enum::GRAY,
                                                   image::Channel_Type_Enum::UINT8,
                                                   class_name(),
                                                   m_logger,
                                                   m_log_mtx );

    // Check for errors
    if( proc_res.has_error() )
    {
        return proc_res.error();
    }
    auto detect_buffer = proc_res.value();

    // Create the opencv image to run detection on
    auto type_code = image::utility::ocv::get_pixel_type_code( detect_buffer.pixel_type(),
                                                               detect_buffer.channel_type() );
    if( type_code.has_error() )
    {
        return outcome::fail( core::error::ErrorCode::INVALID_CONFIGURATION,
                              "Unsupported conversion. ",
                              type_code.error().message() );
    }

    cv::Mat image( detect_buffer.rows(),
                   detect_buffer.cols(),
                   type_code.value(),
                   detect_buffer.data() );
    tmns::log::info( ADD_CURRENT_LOC(), image::utility::ocv::opencv_type_to_string( type_code.value() ) );

    auto score_type = cv::ORB::HARRIS_SCORE;
    if( m_config->score_type() == "FAST" )
    {
        score_type = cv::ORB::FAST_SCORE;
    }
    else if( m_config->score_type() == "HARRIS" )
    {
        // do nothing
    }
    else
    {
        tmns::log::warn( ADD_CURRENT_LOC(),
                         "Unable to determine desired score type (actual: ",
                         m_config->score_type(), ", reverting to HARRIS instead." );
    }

    // Build the feature detector
    int max_points = ( max_points_override > 0 ) ? max_points_override : m_config->max_features();
    auto detector = cv::ORB::create( max_points,
                                     m_config->scale_factor(),
                                     m_config->num_pyr_levels(),
                                     m_config->edge_threshold(),
                                     m_config->base_pyramid_level(),
                                     m_config->wta_k(),
                                     score_type,
                                     m_config->patch_size(),
                                     m_config->fast_threshold() );

    
    // Run detect
    tmns::log::trace( "Calling detect" );
    std::vector<cv::KeyPoint> kps;
    detector->detect( image, kps );
    tmns::log::trace( "Located ", kps.size(), " keypoints" );

    Interest_Point_List points( kps.size() );
    for( size_t i = 0; i < kps.size(); i++ )
    {
        points.emplace_back( math::Point2f( { kps[i].pt.x, kps[i].pt.y } ),
                             kps[i].size,
                             kps[i].angle,
                             kps[i].response,
                             kps[i].octave,
                             kps[i].class_id );
    }
    return outcome::ok<Interest_Point_List>( points );
}

/****************************************/
/*      Perform Feature Extraction      */
/****************************************/
Result<void> Detector_OCV_ORB::perform_feature_extraction( const image::Image_Buffer&    image_buffer,
                                                           std::vector<Interest_Point>&  interest_points,
                                                           bool                          cast_if_ctype_unsupported )
{
    // Process the image
    auto proc_res = utility::prepare_image_buffer( image_buffer,
                                                   cast_if_ctype_unsupported,
                                                   image::Pixel_Format_Enum::GRAY,
                                                   image::Channel_Type_Enum::UINT8,
                                                   class_name(),
                                                   m_logger,
                                                   m_log_mtx );

    // Check for errors
    if( proc_res.has_error() )
    {
        return proc_res.error();
    }
    auto detect_buffer = proc_res.value();

    // Create the opencv image to run detection on
    auto type_code = image::utility::ocv::get_pixel_type_code( detect_buffer.pixel_type(),
                                                               detect_buffer.channel_type() );
    if( type_code.has_error() )
    {
        return outcome::fail( core::error::ErrorCode::INVALID_CONFIGURATION,
                              "Unsupported conversion. ",
                              type_code.error().message() );
    }

    cv::Mat image( detect_buffer.rows(),
                   detect_buffer.cols(),
                   type_code.value(),
                   detect_buffer.data() );
    tmns::log::info( ADD_CURRENT_LOC(),
                     image::utility::ocv::opencv_type_to_string( type_code.value() ) );

    auto score_type = cv::ORB::HARRIS_SCORE;
    if( m_config->score_type() == "FAST" )
    {
        score_type = cv::ORB::FAST_SCORE;
    }
    else if( m_config->score_type() == "HARRIS" )
    {
        // do nothing
    }
    else
    {
        tmns::log::warn( ADD_CURRENT_LOC(),
                         "Unable to determine desired score type (actual: ",
                         m_config->score_type(), ", reverting to HARRIS instead." );
    }

    // Build the feature detector
    auto detector = cv::ORB::create( m_config->max_features(),
                                     m_config->scale_factor(),
                                     m_config->num_pyr_levels(),
                                     m_config->edge_threshold(),
                                     m_config->base_pyramid_level(),
                                     m_config->wta_k(),
                                     score_type,
                                     m_config->patch_size(),
                                     m_config->fast_threshold() );

    
    // Run opencv compute method
    {
        std::unique_lock<std::mutex> lck( m_log_mtx );
        m_logger.trace( "Running Compute" );
    }
    std::vector<cv::KeyPoint> kps;
    cv::Mat descriptors;
    for( const auto& ip : interest_points )
    {
        kps.push_back( cv::KeyPoint( cv::Point2f( ip.pixel_loc().x(),
                                                  ip.pixel_loc().y() ),
                                      ip.scale(),
                                      ip.angle_radians(),
                                      ip.response(),
                                      ip.octave(),
                                      ip.class_id() ) );
    }
    detector->compute( image, kps, descriptors );

    if( kps.size() != descriptors.rows )
    {
        return outcome::fail( core::error::ErrorCode::UNKNOWN,
                              "Descriptors not the same size as keypoints" );
    }

    // Check keypoints
    std::vector<Interest_Point> output_ips;
    for( size_t x = 0; x < kps.size(); x++ )
    {
        output_ips.push_back( Interest_Point( math::Point2f( { kps[x].pt.x,
                                                               kps[x].pt.y } ),
                                              kps[x].size,
                                              kps[x].angle,
                                              kps[x].response,
                                              kps[x].octave,
                                              kps[x].class_id ) );

        output_ips.back().descriptors().clear();
        for( int c = 0; c < descriptors.cols; c++ )
        {
            output_ips.back().descriptors().push_back( descriptors.at<float>( x, c ) );
        }
    }

    {
        std::unique_lock<std::mutex> lck( m_log_mtx );
        m_logger.trace( "Descriptor size. Cols: ", descriptors.cols,
                        ", Rows: ", descriptors.rows );
    }
    return outcome::ok();
}

/****************************/
/*    Get the class name    */
/****************************/
std::string Detector_OCV_ORB::class_name() const
{
    return "Detector_OCV_ORB";
}

/************************************/
/*      Generate New Instance       */
/************************************/
Result<Detector_Base::ptr_t> Detector_Generator_OCV_ORB::generate( Detector_Config_Base::ptr_t config )
{
    // Check if the detector config ORB
    bool same = ( dynamic_cast<Detector_Config_OCV_ORB*>( config.get() ) != nullptr );
    if( same )
    {
        return outcome::ok<Detector_Base::ptr_t>( std::make_shared<Detector_OCV_ORB>( config ) );
    }
    else
    {
        return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                              "Config doesn't match" );
    }
}

} // End of tmns::feature::ocv namespace