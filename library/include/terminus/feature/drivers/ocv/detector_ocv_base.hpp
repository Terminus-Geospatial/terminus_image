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
 * @file    interest_point_detector_OpenCV.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Libraries
#include "../../detector_Base.hpp"
#include "config/detector_Config_OCV_Base.hpp"

namespace tmns::feature::ocv {

class detector_OCV_Base : public detector_Base
{
    public:

        /**
         * Constructor
         */
        detector_OCV_Base( detector_Config_Base::ptr_t config );

        /**
         * Destructor
        */
        ~detector_OCV_Base() override = default;

    protected:

        /**
         * Get the internal configuration instance
        */
        detector_Config_OCV_Base::ptr_t get_ocv_detector_config() const;

    private:

        /// Configuration
        detector_Config_OCV_Base::ptr_t m_config;

}; // End of class detector_OCV_Base

} // End of tmns::feature::ocv namespace