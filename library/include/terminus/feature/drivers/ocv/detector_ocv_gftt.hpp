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
 * @file    detector_OCV_GFFT.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Feature Libraries
#include "../../detector_generator_base.hpp"
#include "config/detector_config_ocv_gftt.hpp"
#include "detector_ocv_base.hpp"

namespace tmns::feature::ocv {

/**
 * OpenCV implementation of the Shi-Thomasi Good-Features-To-Track
 * algorithm.
*/
class detector_OCV_GFTT : public detector_OCV_Base
{
    public:

        /**
         * Default Constructor
         */
        detector_OCV_GFTT();

        /**
         * Parameterized Constructor
        */
        detector_OCV_GFTT( detector_Config_Base::ptr_t config );

        /**
         * Destructor
        */
        ~detector_OCV_GFTT() override = default;

        /**
         * Process the image and detect keypoints
         */
        Result<interest_point_List> process_image( const image::Image_Buffer& image,
                                                   bool                       cast_if_ctype_unsupported,
                                                   int                        max_points_override ) override;

        /**
         * Get the class name
         */
        std::string class_name() const override;

    private:

        /// Configuration
        detector_Config_OCV_GFTT::ptr_t m_config { nullptr };

}; // End of detector_OCV_GFTT class


class detector_Generator_OCV_GFTT : public detector_Generator_Base
{
    public:

        /**
         * Build a new instance of the feature detector
        */
        Result<detector_Base::ptr_t> generate( detector_Config_Base::ptr_t config ) override;

}; // End of detector_Generator_OCV_GFTT

} // End of tmns::feature::ocv namespace