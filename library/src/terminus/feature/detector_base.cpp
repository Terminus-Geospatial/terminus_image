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
 * @file    detector_Base.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include <terminus/feature/detector_base.hpp>

namespace tmns::feature {

/********************************/
/*          Constructor         */
/********************************/
detector_Base::detector_Base( detector_Config_Base::ptr_t config )
 : m_logger( config->logger_name() ),
   m_config( config )
{
}

/****************************************/
/*      Perform Feature Extraction      */
/****************************************/
Result<void> detector_Base::perform_feature_extraction( [[maybe_unused]] const image::Image_Buffer&    image_data,
                                                        [[maybe_unused]] std::vector<interest_point>&  interest_point,
                                                        [[maybe_unused]] bool                          cast_if_ctype_unsupported )
{
    return outcome::fail( error::Error_Code::NOT_IMPLEMENTED,
                          "detector_Base class does not implement 'perform_feature_extraction()'",
                          "Select a supported feature detector." );
}

} // End of tmns::feature namespace