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
 * @file    Memory_Driver_Manager.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

/// External Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/outcome/Result.hpp>

/// Terminus Libraries
#include "../Image_Resource_Memory.hpp"
#include "opencv/Image_Resource_Memory_OpenCV_Factory.hpp"

// C++ Libraries
#include <deque>
#include <filesystem>
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

        /// Type for Drivers
        typedef Image_Resource_Memory::ptr_t  DriverT;

        /// Type for Driver Factories
        typedef Driver_Factory_Base::ptr_t FactoryT;

        /**
         * Create a Driver-Manager using the default configuration
         *
         * @note The order by which a driver is chosen is the order it is registered.
        */
        static Memory_Driver_Manager::ptr_t create_read_defaults()
        {
            // Create new instance
            auto instance = Memory_Driver_Manager::ptr_t( new Memory_Driver_Manager() );

            // Register each driver
            instance->register_read_driver_factory( std::make_shared<ocv::Image_Resource_Memory_OpenCV_Factory>() );

            return instance;
        }

        /**
         * Add a read driver to the list
        */
        void register_read_driver_factory( FactoryT instance );

        /**
         * Add a write driver to the list
        */
        void register_write_driver_factory( FactoryT instance );

        /**
         * Select a driver based on the file.
        */
        Result<DriverT> pick_read_driver( const std::filesystem::path& pathname ) const;

        /**
         * Select a driver based on the file.
        */
        Result<DriverT> pick_write_driver( const std::filesystem::path& pathname ) const;

    private:

        /// Default Constructor
        Memory_Driver_Manager() = default;

        /// List of Drivers
        std::deque<FactoryT> m_read_driver_factories;
        std::deque<FactoryT> m_write_driver_factories;

}; // End of Memory_Driver_Manager Class

} // end of tmns::image::io namespace