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
 * @file    camera_model_factory.cpp
 * @author  Marvin Smith
 * @date    9/23/2023
 */
#include <terminus/geography/camera/camera_model_factory.hpp>

// Terminus Libraries
#include <terminus/geography/camera/camera_model_isis.hpp>

namespace tmns::geo::cam {

/********************************************/
/*          Create from Metadata            */
/********************************************/
Result<camera_model_base::ptr_t>  camera_model_factory::create_from_metadata( image::meta::Metadata_Container_Base::ptr_t container )
{
    // Check the container isn't null
    if( container == nullptr )
    {
        return outcome::fail( error::Error_Code::UNINITIALIZED,
                              "Container cannot be null" );
    }

    // Check driver type
    auto driver_type = container->get<std::string>( "file_driver" );
    if( driver_type.has_error() )
    {
        return outcome::fail( error::Error_Code::NOT_FOUND,
                              "Container has no key 'file_driver'. Cannot determine driver type." );
    }

    // Check if ISIS
    if( driver_type.value() == "ISIS" )
    {
        return camera_model_ISIS::create_from_metadata( container );
    }

    return outcome::fail( error::Error_Code::DRIVER_NOT_FOUND,
                          "Unsupported driver-type: '", driver_type.value(), "'" );
}

} // End of tmns::geo::cam namespace