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
#include "detector_Config_Base.hpp"

namespace tmns::feature {

/**********************************************************/
/*          Check if Descriptors are supported            */
/**********************************************************/
bool detector_Config_Base::supports_feature_descriptors() const
{
    return false;
}

} // End of tmns::feature namespace