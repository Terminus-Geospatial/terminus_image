/**
 * @file    detector_Config_OCV_Base.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Feature Libraries
#include "../../../detector_Config_Base.hpp"

namespace tmns::feature::ocv {

/**
 * @class detector_Config_OCV_Base
 *
 * Base configuration for all OpenCV-based feature detectors
*/
class detector_Config_OCV_Base : public detector_Config_Base
{
    public:

        /**
         * Constructor
        */
        detector_Config_OCV_Base();

    private:


}; // End of class detector_Config_OCV_Base

} // End of tmns::feature::ocv namespace