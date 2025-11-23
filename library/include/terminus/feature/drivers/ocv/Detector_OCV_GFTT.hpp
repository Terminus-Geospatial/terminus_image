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
 * @file    Detector_OCV_GFFT.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Feature Libraries
#include "../../Detector_Generator_Base.hpp"
#include "config/Detector_Config_OCV_GFTT.hpp"
#include "Detector_OCV_Base.hpp"

namespace tmns::feature::ocv {

/**
 * OpenCV implementation of the Shi-Thomasi Good-Features-To-Track
 * algorithm.
*/
class Detector_OCV_GFTT : public Detector_OCV_Base
{
    public:

        /**
         * Default Constructor
         */
        Detector_OCV_GFTT();

        /**
         * Parameterized Constructor
        */
        Detector_OCV_GFTT( Detector_Config_Base::ptr_t config );

        /**
         * Destructor
        */
        ~Detector_OCV_GFTT() override = default;

        /**
         * Process the image and detect keypoints
         */
        Result<Interest_Point_List> process_image( const image::Image_Buffer& image,
                                                   bool                       cast_if_ctype_unsupported,
                                                   int                        max_points_override ) override;

        /**
         * Get the class name
         */
        std::string class_name() const override;

    private:

        /// Configuration
        Detector_Config_OCV_GFTT::ptr_t m_config { nullptr };

}; // End of Detector_OCV_GFTT class


class Detector_Generator_OCV_GFTT : public Detector_Generator_Base
{
    public:

        /**
         * Build a new instance of the feature detector
        */
        Result<Detector_Base::ptr_t> generate( Detector_Config_Base::ptr_t config ) override;

}; // End of Detector_Generator_OCV_GFTT

} // End of tmns::feature::ocv namespace