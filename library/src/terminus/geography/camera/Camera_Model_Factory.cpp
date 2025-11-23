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
 * @file    Camera_Model_Factory.cpp
 * @author  Marvin Smith
 * @date    9/23/2023
 */
#include "Camera_Model_Factory.hpp"

// Terminus Libraries
#include <terminus/geography/camera/Camera_Model_ISIS.hpp>

namespace tmns::geo::cam {

/********************************************/
/*          Create from Metadata            */
/********************************************/
Result<Camera_Model_Base::ptr_t>  Camera_Model_Factory::create_from_metadata( image::meta::Metadata_Container_Base::ptr_t container )
{
    // Check the container isn't null
    if( container == nullptr )
    {
        return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                              "Container cannot be null" );
    }

    // Check driver type
    auto driver_type = container->get<std::string>( "file_driver" );
    if( driver_type.has_error() )
    {
        return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                              "Container has no key 'file_driver'. Cannot determine driver type." );
    }

    // Check if ISIS
    if( driver_type.value() == "ISIS" )
    {
        return Camera_Model_ISIS::create_from_metadata( container );
    }

    return outcome::fail( core::error::ErrorCode::DRIVER_NOT_FOUND,
                          "Unsupported driver-type: '", driver_type.value(), "'" );
}

} // End of tmns::geo::cam namespace