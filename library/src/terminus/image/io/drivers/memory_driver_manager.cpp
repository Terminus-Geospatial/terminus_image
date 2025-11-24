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
 * @file    memory_driver_manager.cpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#include <terminus/image/io/drivers/memory_driver_manager.hpp>

namespace tmns::image::io {

/************************************************************************************/
/*          Pick the appropriate driver in the list for processing the dataset      */
/************************************************************************************/
Result<Memory_Driver_Manager::DriverT> Memory_Driver_Manager::pick_read_driver( const std::filesystem::path& pathname ) const
{
    for( const auto& factory : m_read_driver_factories )
    {
        if( factory->is_read_image_supported( pathname ) )
        {
            auto new_driver = factory->create_read_driver( pathname );
            if( new_driver.has_error() )
            {
                return outcome::fail( new_driver.assume_error() );
            }
            auto driver_ptr = std::dynamic_pointer_cast<Image_Resource_Memory>( new_driver.assume_value() );
            return outcome::ok<Memory_Driver_Manager::DriverT>( std::move( driver_ptr ) );
        }
    }
    return outcome::fail( error::Error_Code::DRIVER_NOT_FOUND );
}

} // End of tmns::image namespace