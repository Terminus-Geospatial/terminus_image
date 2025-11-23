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
 * @file    extract_descriptors.hpp
 * @author  Marvin Smith
 * @date    9/2/2023
*/
#pragma once

// Terminus Feature Libraries
#include "Detector_Factory.hpp"

// C++ Libraries

namespace tmns::feature {

/**
 * Extract descriptors from the image.
 */
template <typename ImageT>
Result<void> extract_descriptors( const image::Image_Base<ImageT>&  image,
                                  const Detector_Base::ptr_t        detector,
                                  const core::Session_Context&      session_context,
                                  std::vector<Interest_Point>&      interest_points )
{
    if( !detector )
    {
        return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                              "Detector is unitialized." );
    }
    return detector->extract_descriptors( image.impl(),
                                          interest_points,
                                          true );
}

/**
 * Run the feature-extraction algorithm, but store the keypoints within the image
 * 
 * @param image
 * @param detector_config
 * @param session_context
 * @param detector_factory
*/
template <typename ImageT>
Result<void> extract_descriptors( image::Image_Base<ImageT>&    image,
                                  Detector_Config_Base::ptr_t   detector_config,
                                  const core::Session_Context&  session_context,
                                  Detector_Factory::ptr_t       detector_factory = Detector_Factory::create_default_instance() )
{
    tmns::log::trace( ADD_CURRENT_LOC(), "Start of method." );

    // Create interest point detector
    auto detector = detector_factory->create_extractor( detector_config );
    if( detector.has_error() )
    {
        return outcome::fail( core::error::ErrorCode::DRIVER_NOT_FOUND,
                              "Driver is not found: ", detector.error().message() );
    }

    return extract_descriptors( image,
                                detector.value(),
                                session_context,
                                image.interest_points() );

} // End of detect_interest_points method

} // End of tmns::feature namespace