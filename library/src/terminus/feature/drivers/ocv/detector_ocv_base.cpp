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
 * @file    detector_OCV_Base.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include "detector_OCV_Base.hpp"

namespace tmns::feature::ocv {

/*********************************/
/*          Constructor          */
/*********************************/
detector_OCV_Base::detector_OCV_Base( detector_Config_Base::ptr_t config )
  : detector_Base( config ),
    m_config( config )
{
}

/********************************************/
/*    Get the OCV Detector Config Object    */
/********************************************/
detector_Config_OCV_Base::ptr_t detector_OCV_Base::get_ocv_detector_config() const
{
    return m_config;
}

} // End of tmns::feature::ocv namespace