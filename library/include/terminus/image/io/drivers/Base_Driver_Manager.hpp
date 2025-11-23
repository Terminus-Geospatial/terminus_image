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
 * @file    Base_Driver_Manager.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

/// External Terminus Libraries
#include <terminus/outcome/Result.hpp>

/// Terminus Libraries
#include "../Image_Resource_Base.hpp"
#include "../../error/ErrorCategory.hpp"

// C++ Libraries
#include <deque>
#include <memory>

namespace tmns::image::io {

/**
 * Relatively simple class for storing file input and output drivers.
*/
class Memory_Driver_Manager
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Memory_Driver_Manager> ptr_t;

        /**
         * Create a Driver-Manager using the default configuration
         *
         * @note The order by which a driver is chosen is the order it is registered.
        */
        static constexpr Memory_Driver_Manager::ptr_t create_defaults()
        {
            // Create new instance
            auto instance = std::make_shared<Memory_Driver_Manager>();

            // Register each driver
            instance->register_driver( std::make_shared<ovc::Image_Resource_Memory_OpenCV>() );

            return instance;
        }

        /**
         * Select a driver based on the file.
        */
        Result<Memory_Driver_Manager::ptr_t> pick_driver( const std::filesystem::path& pathname ) const;

    private:

        /// Default Constructor
        Memory_Driver_Manager() = default;

        /// List of Drivers
        std::deque<Image_Resource_Memory::ptr_t> m_drivers;

}; // End of Memory_Driver_Manager Class

} // end of tmns::image::io namespace