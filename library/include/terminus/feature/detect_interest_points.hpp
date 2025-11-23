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
 * @file    detect_interest_points.hpp
 * @author  Marvin Smith
 * @date    7/20/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/work/Work_Queues.hpp>
#include <terminus/core/Session_Context.hpp>

// Terminus Feature Libraries
#include "utility/Interest_Detection_Queue.hpp"
#include "Detector_Factory.hpp"

// C++ Libraries

namespace tmns::feature {

/**
 * Perform feature detection on an image, returning the interest points
 */
template <typename ImageT>
Result<std::vector<Interest_Point>> detect_interest_points( const image::Image_Base<ImageT>&  image,
                                                            const Detector_Base::ptr_t        detector,
                                                            const core::Session_Context&      session_context )
{
    // Build work queue for writing jobs in order
    auto write_pool = std::make_shared<core::work::Work_Queue_Ordered>( 1 );

    Interest_Point_List ip_list;

    utility::Interest_Detection_Queue<ImageT> detect_queue( image,
                                                            detector,
                                                            write_pool,
                                                            ip_list,
                                                            detector->config()->tile_size_pixels(),
                                                            detector->config()->max_features() );
  
    tmns::log::debug( "Waiting for threads to complete." );
    
    detect_queue.join_all();
    
    write_pool->join_all();
    
    return ip_list;
}

/**
 * Run the detector, but store the keypoints within the image
 * 
 * @param image
 * @param detector_config
 * @param session_context
 * @param detector_factory
*/
template <typename ImageT>
Result<void> detect_interest_points( image::Image_Base<ImageT>&    image,
                                     Detector_Config_Base::ptr_t   detector_config,
                                     const core::Session_Context&  session_context,
                                     Detector_Factory::ptr_t       detector_factory = Detector_Factory::create_default_instance() )
{
    tmns::log::trace( ADD_CURRENT_LOC(), "Start of method." );

    // Create interest point detector
    auto detector = detector_factory->create_detector( detector_config );
    if( detector.has_error() )
    {
        return outcome::fail( core::error::ErrorCode::DRIVER_NOT_FOUND,
                              "Driver is not found: ", detector.error().message() );
    }

    auto keypoints = detect_interest_points( image,
                                             detector.value(),
                                             session_context );

    if( keypoints.has_error() )
    {
        return outcome::fail( keypoints.error() );
    }
    image.interest_points() = keypoints.value();

    return outcome::ok();

} // End of detect_interest_points method

} // End of tmns::feature namespace