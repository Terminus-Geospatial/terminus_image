/**
 * @file    Memory_Driver_Manager.cpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#include "Memory_Driver_Manager.hpp"

namespace tmns::image {

/************************************************************************************/
/*          Pick the appropriate driver in the list for processing the dataset      */
/************************************************************************************/
Result<Read_Image_Resource_Memory::ptr_t> Disk_Driver_Manager::pick_read_driver( const std::filesystem::path& pathname ) const
{
    for( const auto& factory : m_read_driver_factories )
    {
        if( factory->is_image_supported( pathname ) )
        {
            auto new_driver = factory->create( pathname );
            if( new_driver.has_error() )
            {
                return outcome::fail( new_driver.assume_error() );
            }
            auto driver_ptr = std::dynamic_pointer_cast<Read_Image_Resource_Memory>( new_driver.assume_value() );
            return outcome::ok<Read_Image_Resource_Memory::ptr_t>( std::move( driver_ptr ) );
        }
    }
    return outcome::fail( error::ErrorCode::DRIVER_NOT_FOUND );
}

} // End of tmns::image namespace