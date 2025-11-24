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
 * @file    disk_driver_manager.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

/// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/image/io/image_resource_disk.hpp>
#include <terminus/image/io/drivers/gdal/image_resource_disk_gdal_factory.hpp>
#include <terminus/image/io/drivers/driver_factory_base.hpp>

// C++ Libraries
#include <deque>
#include <memory>

namespace tmns::image::io {

/**
 * Relatively simple class for storing file input and output drivers.
*/
class Disk_Driver_Manager : boost::noncopyable
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Disk_Driver_Manager> ptr_t;

        /// Type for Read Drivers
        typedef Image_Resource_Disk::ptr_t  DriverT;

        /// Type for Read Driver Factories
        typedef Driver_Factory_Base::ptr_t FactoryT;

        /**
         * Create a Driver-Manager using the default configuration
         *
         * @note The order by which a driver is chosen is the order it is registered.
        */
        static Disk_Driver_Manager::ptr_t create_read_defaults()
        {
            // Create new instance
            auto instance = Disk_Driver_Manager::ptr_t( new Disk_Driver_Manager() );

            // Register each driver
            instance->register_read_driver_factory( std::make_shared<gdal::Image_Resource_Disk_GDAL_Factory>() );

            return instance;
        }

        /**
         * Create a Driver-Manager using the default configuration
         *
         * @note The order by which a driver is chosen is the order it is registered.
        */
        static Disk_Driver_Manager::ptr_t create_write_defaults()
        {
            // Create new instance
            auto instance = Disk_Driver_Manager::ptr_t( new Disk_Driver_Manager() );

            // Register each driver
            instance->register_write_driver_factory( std::make_shared<gdal::Image_Resource_Disk_GDAL_Factory>() );

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
        Result<DriverT> pick_write_driver( const std::filesystem::path&             pathname,
                                           const Image_Format&                      output_format,
                                           const std::map<std::string,std::string>& write_options,
                                           const math::Size2i&                      block_size ) const;

    private:

        /// Default Constructor
        Disk_Driver_Manager() = default;

        /// List of Drivers
        std::deque<FactoryT> m_read_driver_factories;
        std::deque<FactoryT> m_write_driver_factories;

}; // End of Disk_Driver_Manager Class

} // end of tmns::image::io namespace