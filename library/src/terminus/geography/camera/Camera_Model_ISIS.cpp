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
 * @file    Camera_Model_ISIS.cpp
 * @author  Marvin Smith
 * @date    9/15/2023
*/
#include "Camera_Model_ISIS.hpp"

namespace tmns::geo::cam {

/********************************/
/*          Constructor         */
/********************************/
Camera_Model_ISIS::Camera_Model_ISIS( const std::filesystem::path& pathname )
{

}

/****************************************/
/*      Convert 3d point into pixel     */
/****************************************/
Result<math::Point2d> Camera_Model_ISIS::point_to_pixel( const math::Point3d& point ) const
{
    return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                          "Not Implemented Yet." );
}

/************************************************/
/*          Convert Pixel into 3D Point         */
/************************************************/
Result<math::Vector3d> Camera_Model_ISIS::pixel_to_vector( const math::Point2d& pixel ) const
{
    return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                          "Not Implemented Yet." );
}

/********************************************/
/*          Get the Camera Origin           */
/********************************************/
Result<math::Point3d> Camera_Model_ISIS::camera_origin( const math::Point2d& pixel ) const
{
    return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                          "Not Implemented Yet." );
}

/********************************************/
/*      Get Camera Model Type String        */
/********************************************/
std::string Camera_Model_ISIS::type() const
{
    return "ISIS";
}

/************************************/
/*          Get Camera Pose         */
/************************************/
Result<math::Quaternion> Camera_Model_ISIS::camera_pose( const math::Point2d& pixel ) const
{
    return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                          "Not Implemented Yet." );
}

/****************************************/
/*          Print to Log String         */
/****************************************/
std::string Camera_Model_ISIS::to_string( size_t offset ) const
{
    std::stringstream sout;
    std::string gap( offset, ' ' );

    sout << gap << " - Camera_Model_ISIS:" << std::endl;
    sout << gap << "    - type: " << type() << std::endl;
    

    return sout.str();
}

/****************************************************************/
/*          Create Camera Model from Metadata Container         */
/****************************************************************/
Result<Camera_Model_Base::ptr_t>  Camera_Model_ISIS::create_from_metadata( const image::meta::Metadata_Container_Base::ptr_t container )
{
    // find the pathname
    auto pathname = container->get<std::filesystem::path>( "pathname" );
    if( pathname.has_error() )
    {
        return outcome::fail( core::error::ErrorCode::PARSING_ERROR,
                              "Container must have 'pathname' key in order to load ISIS mode." );
    }

    // Otherwise, return new model
    auto new_camera = std::make_shared<Camera_Model_ISIS>( pathname.value() );

    // Make sure model loaded without failure


    return outcome::ok<Camera_Model_Base::ptr_t>( new_camera );
}

} // End of tmns::geo::cam namespace