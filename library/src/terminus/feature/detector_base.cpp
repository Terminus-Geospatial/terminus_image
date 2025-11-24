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
 * @file    detector_base.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include <terminus/feature/detector_base.hpp>

namespace tmns::feature {

/********************************/
/*          Constructor         */
/********************************/
Detector_Base::Detector_Base( Detector_Config_Base::ptr_t config )
 : m_logger( config->logger_name() ),
   m_config( config )
{
}

/****************************************/
/*      Perform Feature Extraction      */
/****************************************/
Result<void> Detector_Base::perform_feature_extraction( [[maybe_unused]] const image::Image_Buffer&    image_data,
                                                        [[maybe_unused]] std::vector<Interest_Point>&  interest_point,
                                                        [[maybe_unused]] bool                          cast_if_ctype_unsupported )
{
    return outcome::fail( error::Error_Code::NOT_IMPLEMENTED,
                          "Detector_Base class does not implement 'perform_feature_extraction()'",
                          "Select a supported feature detector." );
}

} // End of tmns::feature namespace