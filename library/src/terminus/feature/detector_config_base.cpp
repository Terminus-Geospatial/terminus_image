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
 * @file    detector_Config_Base.cpp
 * @author  Marvin Smith
 * @date    9/2/2023
*/
#include <terminus/feature/detector_config_base.hpp>

namespace tmns::feature {

/**********************************************************/
/*          Check if Descriptors are supported            */
/**********************************************************/
bool Detector_Config_Base::supports_feature_descriptors() const
{
    return false;
}

} // End of tmns::feature namespace