/**
 * @file    Detector_Config_OCV_Base.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Feature Libraries
#include "../../../Detector_Config_Base.hpp"

namespace tmns::feature::ocv {

/**
 * @class Detector_Config_OCV_Base
 *
 * Base configuration for all OpenCV-based feature detectors
*/
class Detector_Config_OCV_Base : public Detector_Config_Base
{
    public:

        /**
         * Constructor
        */
        Detector_Config_OCV_Base();

    private:


}; // End of class Detector_Config_OCV_Base

} // End of tmns::feature::ocv namespace