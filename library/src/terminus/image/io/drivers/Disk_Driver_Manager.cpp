/**
 * @file    Disk_Driver_Manager.cpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#include "Disk_Driver_Manager.hpp"

// Terminus Libraries
#include <terminus/log/utility.hpp>

namespace tmns::image::io {

/************************************************************************************/
/*          Pick the appropriate driver in the list for processing the dataset      */
/************************************************************************************/
Result<Image_Resource_Disk::ptr_t> Disk_Driver_Manager::pick_read_driver( const std::filesystem::path& pathname ) const
{
    for( const auto& factory : m_read_driver_factories )
    {
        if( factory->is_read_image_supported( pathname ) )
        {
            auto new_driver = factory->create_read_driver( pathname );
            if( new_driver.has_error() )
            {
                return outcome::fail( core::error::ErrorCode::DRIVER_NOT_FOUND,
                                      "Failed to find new write driver: ",
                                      "MSG: ", new_driver.error().message() );
            }
            auto driver_ptr = std::dynamic_pointer_cast<Image_Resource_Disk>( new_driver.assume_value() );
            return outcome::ok<Image_Resource_Disk::ptr_t>( std::move( driver_ptr ) );
        }
    }
    return outcome::fail( core::error::ErrorCode::DRIVER_NOT_FOUND,
                          "No driver found for path." );
}

/************************************************************************************/
/*          Pick the appropriate driver in the list for processing the dataset      */
/************************************************************************************/
Result<Image_Resource_Disk::ptr_t> Disk_Driver_Manager::pick_write_driver( const std::filesystem::path&             pathname,
                                                                           const Image_Format&                      output_format,
                                                                           const std::map<std::string,std::string>& write_options,
                                                                           const math::Size2i&                      block_size ) const
{
    tmns::log::trace( ADD_CURRENT_LOC(), "Start of Method" );
    for( const auto& factory : m_write_driver_factories )
    {
        if( factory->is_write_image_supported( pathname ) )
        {
            auto new_driver = factory->create_write_driver( pathname,
                                                            output_format,
                                                            write_options,
                                                            block_size );
            if( new_driver.has_error() )
            {
                return outcome::fail( core::error::ErrorCode::DRIVER_NOT_FOUND,
                                      "Failed to find new write driver: ",
                                      "MSG: ", new_driver.error().message() );
            }
            auto driver_ptr = std::dynamic_pointer_cast<Image_Resource_Disk>( new_driver.assume_value() );
            return outcome::ok<Image_Resource_Disk::ptr_t>( std::move( driver_ptr ) );
        }
    }
    return outcome::fail( core::error::ErrorCode::DRIVER_NOT_FOUND );
}

/************************************************/
/*      Add new factory to driver manager       */
/************************************************/
void Disk_Driver_Manager::register_read_driver_factory( Driver_Factory_Base::ptr_t new_driver )
{
    m_read_driver_factories.push_back( new_driver );
}

/************************************************/
/*      Add new factory to driver manager       */
/************************************************/
void Disk_Driver_Manager::register_write_driver_factory( Driver_Factory_Base::ptr_t new_driver )
{
    m_write_driver_factories.push_back( new_driver );
}

} // End of tmns::image::io namespace