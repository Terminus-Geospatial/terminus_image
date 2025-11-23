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
 * @file    Camera_Model_ISIS.hpp
 * @author  Marvin Smith
 * @date    9/15/2023
 */
#pragma once

// Terminus GEO Namespace
#include <terminus/coordinate/Datum.hpp>
#include "Camera_Model_Base.hpp"

// Terminus Libraries
#include <terminus/image/metadata/Metadata_Container_Base.hpp>

// C++ Libraries
#include <filesystem>

namespace tmns::geo::cam {

class Camera_Model_ISIS : public Camera_Model_Base
{
    public:

        /**
         * Constructor
         */
        Camera_Model_ISIS( const std::filesystem::path& cube_path );

        /**
         * Destructor
         */
        ~Camera_Model_ISIS() override = default;

        /**
         * Transform a coordinate from 3d cartesian into pixel coordinates.  Pixel coordinates are 2-dimensional values
         * in real (aka floating-point) representation.  This allows for sub-pixel accuracy.  To get an indexible conversion,
         * simply convert to an integer space noting if the "center" is the top-left or center. 
         */
        Result<math::Point2d> point_to_pixel( const math::Point3d& point ) const override;

        /**
         * Returns a (normalized) pointing vector from the camera center through the position of the pixel 
         * 'pix' on the image plane.
         */
        Result<math::Vector3d> pixel_to_vector( const math::Point2d& pixel ) const override;


        /**
         * Returns the position of the focal point of the camera
         */
        Result<math::Point3d> camera_origin( const math::Point2d& pixel ) const override;

        /**
         * Camera Type
         * Returns "ISIS"
         */
        std::string type() const override;

        /**
         * Get the pose, represented as a quaternion, for the camera at a particular pixel.
         * It represents the rotation from the camera frame to the world frame.  
         * - The input pixel will likely only be used for linescan cameras.
         */
        Result<math::Quaternion> camera_pose( const math::Point2d& pixel ) const override;

        
        /**
         * Returns the number of lines is the ISIS cube  (Rows)
         */
        int lines() const;

        /**
         * Returns the number of samples in the ISIS cube
         */
        int samples() const;

        /**
         * Returns the serial number of the ISIS cube
         */
        std::string serial_number() const;

        /**
         * Returns the ephemeris time for a pixel
         */
        double ephemeris_time( const math::Point2d& pix = math::Point2d() ) const;

        /**
         * Sun position in the target frame's inertial frame
         */
        math::Vector3d sun_position( const math::Point2d& pixel = math::Point2d() ) const;

        /**
         * The three main radii that make up the spheroid. Z is out the polar region
         */
        math::Vector3d target_radii() const;

        /**
         * Spheroid name
         */
        std::string target_name() const;

        /**
         * The datum
         */
        coordinate::Datum get_datum( bool use_sphere_for_non_earth ) const;

        /**
         * Print to string
         */
        std::string to_string( size_t offset ) const override;

        /**
         * Generate Camera-Model from Metadata-Container-Base
        */
        static Result<Camera_Model_Base::ptr_t> create_from_metadata( const image::meta::Metadata_Container_Base::ptr_t container );

}; // End of Camera_Model_ISIS class

} // End of tmns::geo::cam namespace