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
 * @file    Detector_Base.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include "Detector_Base.hpp"

namespace tmns::feature {

/********************************/
/*          Constructor         */
/********************************/
Detector_Base::Detector_Base( Detector_Config_Base::ptr_t config )
 : m_config( config ),
   m_logger( config->logger_name() )
{
}

/****************************************/
/*      Perform Feature Extraction      */
/****************************************/
Result<void> Detector_Base::perform_feature_extraction( const image::Image_Buffer&    image_data,
                                                        std::vector<Interest_Point>&  interest_point,
                                                        bool                          cast_if_ctype_unsupported )
{
    return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED,
                          "Detector_Base class does not implement 'perform_feature_extraction()'",
                          "Select a supported feature detector." );
}

} // End of tmns::feature namespace