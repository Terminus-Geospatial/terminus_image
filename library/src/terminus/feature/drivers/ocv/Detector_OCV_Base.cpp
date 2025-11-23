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
 * @file    Detector_OCV_Base.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include "Detector_OCV_Base.hpp"

namespace tmns::feature::ocv {

/*********************************/
/*          Constructor          */
/*********************************/
Detector_OCV_Base::Detector_OCV_Base( Detector_Config_Base::ptr_t config )
  : Detector_Base( config ),
    m_config( config )
{
}

/********************************************/
/*    Get the OCV Detector Config Object    */
/********************************************/
Detector_Config_OCV_Base::ptr_t Detector_OCV_Base::get_ocv_detector_config() const
{
    return m_config;
}

} // End of tmns::feature::ocv namespace