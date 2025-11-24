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
 * @file    detector_Factory.hpp
 * @author  Marvin Smith
 * @date    8/6/2023
*/
#pragma once

// C++ Libraries
#include <map>
#include <memory>

// Terminus Feature Libraries
#include "drivers/ocv/detector_ocv_gftt.hpp"
#include "drivers/ocv/detector_ocv_orb.hpp"

namespace tmns::feature {

class detector_Factory
{
    public:

        /// @brief Pointer Type
        typedef std::shared_ptr<detector_Factory> ptr_t;

        /**
         * Create an empty instance of the factory
        */
        static detector_Factory::ptr_t create_instance();

        /**
         * Create a default instance of the factory
         */
        inline static detector_Factory::ptr_t create_default_instance()
        {
            auto instance = std::make_shared<detector_Factory>();

            // Add the standard generators
            instance->m_generators.push_back( std::make_shared<ocv::detector_Generator_OCV_GFTT>() );
            instance->m_generators.push_back( std::make_shared<ocv::detector_Generator_OCV_ORB>() );

            return instance;
        }

        /**
         * Create a feature detector instance.
         */
        Result<detector_Base::ptr_t> create_detector( detector_Config_Base::ptr_t config ) const;

        /**
         * Create a feature extractor instance.
         */
        Result<detector_Base::ptr_t> create_extractor( detector_Config_Base::ptr_t config ) const;

    private:

        /// List of registered feature detectors
        std::vector<detector_Generator_Base::ptr_t> m_generators;

}; // End of detector_Factory class

} // End of tmns::feature namespace