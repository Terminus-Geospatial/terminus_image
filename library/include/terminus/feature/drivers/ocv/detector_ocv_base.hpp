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
 * @file    interest_point_detector_opencv.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/feature/detector_base.hpp>
#include <terminus/feature/drivers/ocv/config/detector_config_ocv_base.hpp>

namespace tmns::feature::ocv {

class Detector_OCV_Base : public Detector_Base
{
    public:

        /**
         * Constructor
         */
        Detector_OCV_Base( Detector_Config_Base::ptr_t config );

        /**
         * Destructor
        */
        ~Detector_OCV_Base() override = default;

    protected:

        /**
         * Get the internal configuration instance
        */
        Detector_Config_OCV_Base::ptr_t get_ocv_detector_config() const;

    private:

        /// Configuration
        Detector_Config_OCV_Base::ptr_t m_config;

}; // End of class Detector_OCV_Base

} // End of tmns::feature::ocv namespace