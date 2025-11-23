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
 * @file    Detector_Factory.hpp
 * @author  Marvin Smith
 * @date    8/6/2023
*/
#pragma once

// C++ Libraries
#include <map>
#include <memory>

// Terminus Feature Libraries
#include "drivers/ocv/Detector_OCV_GFTT.hpp"
#include "drivers/ocv/Detector_OCV_ORB.hpp"

namespace tmns::feature {

class Detector_Factory
{
    public:

        /// @brief Pointer Type
        typedef std::shared_ptr<Detector_Factory> ptr_t;

        /**
         * Create an empty instance of the factory
        */
        static Detector_Factory::ptr_t create_instance();

        /**
         * Create a default instance of the factory
         */
        inline static Detector_Factory::ptr_t create_default_instance()
        {
            auto instance = std::make_shared<Detector_Factory>();

            // Add the standard generators
            instance->m_generators.push_back( std::make_shared<ocv::Detector_Generator_OCV_GFTT>() );
            instance->m_generators.push_back( std::make_shared<ocv::Detector_Generator_OCV_ORB>() );

            return instance;
        }

        /**
         * Create a feature detector instance.
         */
        Result<Detector_Base::ptr_t> create_detector( Detector_Config_Base::ptr_t config ) const;

        /**
         * Create a feature extractor instance.
         */
        Result<Detector_Base::ptr_t> create_extractor( Detector_Config_Base::ptr_t config ) const;

    private:

        /// List of registered feature detectors
        std::vector<Detector_Generator_Base::ptr_t> m_generators;

}; // End of Detector_Factory class

} // End of tmns::feature namespace