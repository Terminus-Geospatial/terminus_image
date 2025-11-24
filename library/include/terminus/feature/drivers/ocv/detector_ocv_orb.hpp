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
 * @file    detector_OCV_ORB.hpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#pragma once

// Terminus Feature Libraries
#include "../../detector_Generator_Base.hpp"
#include "../../detector_Traits.hpp"
#include "config/detector_Config_OCV_ORB.hpp"
#include "detector_OCV_Base.hpp"

namespace tmns::feature {
namespace ocv {

/**
 * OpenCV implementation of the Oriented Brief Keypoint Detector
 * algorithm.
*/
class detector_OCV_ORB : public detector_OCV_Base
{
    public:

        /**
         * Default Constructor
         */
        detector_OCV_ORB();

        /**
         * Parameterized Constructor
        */
        detector_OCV_ORB( detector_Config_Base::ptr_t config );

        /**
         * Destructor
        */
        ~detector_OCV_ORB() override = default;

        /**
         * Process the image and detect keypoints
         * 
         * @param image Image to run detection on
         * @param cast_if_ctype_unsupported Cast to baseline channel type if input is not supported.
         */
        Result<interest_point_List> process_image( const image::Image_Buffer& image,
                                                   bool                        cast_if_ctype_unsupported,
                                                   int                         max_points_override ) override;
        
        /**
         * Process the image to extract feature descriptors
         */
        Result<void> perform_feature_extraction( const image::Image_Buffer&    image_data,
                                                 std::vector<interest_point>&  interest_point,
                                                 bool                          cast_if_ctype_unsupported ) override;

        /**
         * Get the class name
         */
        std::string class_name() const override;

    private:

        /// Configuration
        detector_Config_OCV_ORB::ptr_t m_config { nullptr };

}; // End of detector_OCV_ORB class


class detector_Generator_OCV_ORB : public detector_Generator_Base
{
    public:

        /**
         * Build a new instance of the feature detector
        */
        Result<detector_Base::ptr_t> generate( detector_Config_Base::ptr_t config ) override;

}; // End of detector_Generator_OCV_ORB

} // End of ocv namespace

namespace trait {

// Create trait enabling feature-extractor
template<>
struct Has_Feature_Extractor<ocv::detector_OCV_ORB> : std::true_type {};

} // End of trait namespace
} // End of tmns::feature namespace