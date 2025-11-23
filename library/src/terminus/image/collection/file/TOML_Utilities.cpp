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
 * @file    TOML_Utilities.cpp
 * @author  Marvin Smith
 * @date    11/9/2023
*/
#include "TOML_Utilities.hpp"

// C++ Libraries
#include <algorithm>

// Terminus Libraries
#include <terminus/geography/camera/Camera_Model_Pinhole.hpp>
#include <terminus/geography/camera/Distortion_Brown_Conrady.hpp>

namespace tmns::image::cx::file {

/****************************************/
/*      Load the Intrinsic Model        */
/****************************************/
Result<geo::cam::Camera_Model_Base::ptr_t> load_intrinsics( toml::table view )
{
    geo::cam::Camera_Model_Base::ptr_t camera;

    // Check if the "type" code is present
    if( !view["camera_type"] )
    {
        return outcome::fail( core::error::ErrorCode::UNDEFINED,
                              "No 'type' code is present." );
    }
    std::string type_str = *view["type"].value<std::string>();
    std::transform( type_str.begin(),
                    type_str.end(),
                    type_str.begin(),
                    ::tolower );

    // Brown-Conrady
    if( type_str == "pinhole" )
    {
        try 
        {
            // @todo:  Implement the actual camera origin
            math::Point3d camera_origin;
            if( !!view["camera_origin_valid"] )
            {

            }

            // Focal Length in pitch units
            math::Point2d focal_length_pitch;
            focal_length_pitch.x() = *view["focal_length_x_pitch"].value<double>();
            focal_length_pitch.y() = *view["focal_length_y_pitch"].value<double>();

            // Check principle point
            math::Point2d principle_point;
            principle_point.x() = *view["principal_point_x_pitch"].value<double>();
            principle_point.y() = *view["principle_point_y_pitch"].value<double>();

            // Get camera axes
            size_t idx = 0;
            math::Vector3d x_axis, y_axis, z_axis;
            if ( auto array = view["camera_x_axis_direction"].as_array() )
            {
                for (const auto& value : *array)
                {
                    x_axis[idx++] = *value.value<double>();
                }
            }
            idx = 0;
            if ( auto array = view["camera_y_axis_direction"].as_array() )
            {
                for (const auto& value : *array)
                {
                    x_axis[idx++] = *value.value<double>();
                }
            }
            idx = 0;
            if ( auto array = view["camera_z_axis_direction"].as_array() )
            {
                for (const auto& value : *array)
                {
                    x_axis[idx++] = *value.value<double>();
                }
            }

            // Grab the pitch
            double pitch = *view["pitch"].value<double>();

            // Create distortion parameters
            if( !view["distortion_type"] )
            {
                return outcome::fail( core::error::ErrorCode::UNDEFINED,
                              "No 'distortion_type' code is present for pinhole." );
            }
            std::string dist_type_str = *view["distortion_type"].value<std::string>();
            std::transform( dist_type_str.begin(),
                            dist_type_str.end(),
                            dist_type_str.begin(),
                            ::tolower );

            geo::cam::Distortion_Base::ptr_t distortion;
            if( dist_type_str == "brown_conrady" )
            {
                // Get the radial distortion
                std::vector<double> k;
                if( !!view["k1"] )
                {
                    k.push_back( *view["k1"].value<double>() );
                }
                if( !!view["k2"] )
                {
                    k.push_back( *view["k2"].value<double>() );
                }
                if( !!view["k3"] )
                {
                    k.push_back( *view["k3"].value<double>() );
                }
                if( !!view["k4"] )
                {
                    k.push_back( *view["k4"].value<double>() );
                }
                
                // Tangential Distortion
                std::vector<double> t;
                if( !!view["t1"] )
                {
                    t.push_back( *view["t1"].value<double>() );
                }
                if( !!view["t2"] )
                {
                    t.push_back( *view["t1"].value<double>() );
                }
                
                double phi_radians = *view["phi_radians"].value<double>();

                distortion = std::make_unique<geo::cam::Distortion_Brown_Conrady>( principle_point,
                                                                                   k,
                                                                                   t,
                                                                                   phi_radians );
            }

            else
            {
                return outcome::fail( core::error::ErrorCode::UNDEFINED,
                                      "Unsupported 'distortion_type' code received: ", dist_type_str );
            }

            // Create Conrady-Model
            camera = std::make_shared<geo::cam::Camera_Model_Pinhole>( camera_origin,
                                                                       focal_length_pitch,
                                                                       principle_point,
                                                                       x_axis,
                                                                       y_axis,
                                                                       z_axis,
                                                                       pitch,
                                                                       std::move( distortion ) );
        }
        catch( std::exception& e )
        {
            return outcome::fail( core::error::ErrorCode::UNDEFINED,
                                  "Caught exception parsing pinhole model. what: ",
                                  e.what() );
        }
    }
    else
    {
        return outcome::fail( core::error::ErrorCode::UNDEFINED,
                              "Unsupported type code '", type_str, "'." );
    }

    // Return the result
    return outcome::ok<geo::cam::Camera_Model_Base::ptr_t>( camera );
}

} // End of tmns::image::cx::file namespace