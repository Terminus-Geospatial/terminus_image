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
 * @file    Camera_Model_Base.hpp
 * @author  Marvin Smith
 * @date    9/15/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/image/metadata/Metadata_Container_Base.hpp>
#include <terminus/math/Point.hpp>
#include <terminus/math/Quaternion.hpp>

// C++ Libraries
#include <memory>

namespace tmns::geo::cam {

/**
 * Base type for all Camera-Model implementations.
 * 
 * If your derived camera model doesn't implement a specific method, ensure you return
 * an Result with an error-code of Not-implemented.
 * 
 * The forward projection method goes from 3d into pixel space.  The inverse projection
 * goes from the camera focal plane, to the world.  The inverse is always much more complex.
*/
class Camera_Model_Base
{
    public:

        using ptr_t = std::shared_ptr<Camera_Model_Base>;

        virtual ~Camera_Model_Base() = default;

        /**
         * Transform a coordinate from 3d cartesian into pixel coordinates.  Pixel coordinates are 2-dimensional values
         * in real (aka floating-point) representation.  This allows for sub-pixel accuracy.  To get an indexible conversion,
         * simply convert to an integer space noting if the "center" is the top-left or center. 
         */
        virtual Result<math::Point2d> point_to_pixel( const math::Point3d& point ) const = 0;


        /**
         * Transform a coordinate from 2d pixel space into a (likely) unit-length vector in 3D space from the origin of the
         * sensor.  
         */
        virtual Result<math::Vector3d> pixel_to_vector( const math::Point2d& pixel ) const = 0;

        /**
         * Get the camera origin in the frame of reference of the camera model.  This method is often used to
         * find the origin of a ray emanating from the focal point of the camera through a pixel on the image
         * plane (e.g. for computing ray-ray intersection in a stereo vision algorithm).
         * - Generally the input pixel is only used for linescan cameras.
         */
        virtual Result<math::Point3d> camera_origin( const math::Point2d& pixel ) const = 0;

        /**
         * Get the "type" string for the specified camera-model.  These must be unique amongst registered camera-model
         * types.
         */
        virtual std::string type() const = 0;

        /**
         * Get the pose, represented as a quaternion, for the camera at a particular pixel.
         * It represents the rotation from the camera frame to the world frame.  
         * - The input pixel will likely only be used for linescan cameras.
         */
        virtual Result<math::Quaternion> camera_pose( const math::Point2d& pixel ) const;

        /**
         * Print a human-readable output of the camera configuration
         */
        virtual std::string to_string( size_t offset ) const = 0;

}; // End of Camera_Model_Base class

} // End of tmns::geo::cam namespace