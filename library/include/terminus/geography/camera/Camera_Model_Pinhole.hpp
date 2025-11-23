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
 * @file    Camera_Model_Pinhole.hpp
 * @author  Marvin Smith
 * @date    11/9/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/geography/camera/Camera_Model_Base.hpp>
#include <terminus/geography/camera/Distortion_Base.hpp>
#include <terminus/geography/camera/Distortion_Null.hpp>
#include <terminus/math/Point.hpp>
#include <terminus/math/Size.hpp>

// C++ Libraries
#include <array>
#include <vector>

namespace tmns::geo::cam {

/**
 * Camera model for the Brown-Conrady camera
*/
class Camera_Model_Pinhole : public Camera_Model_Base
{
    public:

        using ptr_t = std::shared_ptr<Camera_Model_Pinhole>;

        Camera_Model_Pinhole();

        /**
         * Constructor
         */
        Camera_Model_Pinhole( const math::Point3d&    camera_center,
                              const math::Point2d&    focal_length_pitch,
                              const math::Point2d&    principle_point_pitch,
                              const math::Vector3d&   x_axis_direction,
                              const math::Vector3d&   y_axis_direction,
                              const math::Vector3d&   z_axis_direction,
                              double                  pitch      = 1.0,
                              Distortion_Base::ptr_t  distortion = nullptr );
        
        /**
         * Destructor
         */
        ~Camera_Model_Pinhole() override = default;

        /**
         * Transform a coordinate from 3d cartesian into pixel coordinates.  Pixel coordinates are 2-dimensional values
         * in real (aka floating-point) representation.  This allows for sub-pixel accuracy.  To get an indexible conversion,
         * simply convert to an integer space noting if the "center" is the top-left or center. 
         */
        math::Point2d point_to_pixel_no_check( const math::Point3d& point ) const;

        /**
         * Transform a coordinate from 3d cartesian into pixel coordinates.  Pixel coordinates are 2-dimensional values
         * in real (aka floating-point) representation.  This allows for sub-pixel accuracy.  To get an indexible conversion,
         * simply convert to an integer space noting if the "center" is the top-left or center. 
         */
        Result<math::Point2d> point_to_pixel( const math::Point3d& point ) const override;

        /**
         * Set the flag to enable performing the point-to-pixel check
         */
        void set_do_point_to_pixel_check( bool value );

        /// As point_to_pixel, but ignoring any lens distortion.
        math::Point2d point_to_pixel_no_distortion( const math::Point3d& point ) const;

        /**
         * Transform a coordinate from 2d pixel space into a (likely) unit-length vector in 3D space from the origin of the
         * sensor.  
         */
        Result<math::Vector3d> pixel_to_vector( const math::Point2d& pixel ) const override;

        /**
         * Check if the projection is valid
         */
        bool projection_valid( const math::Point3d& point ) const;

        /**
         * Get the principle point in pitch units
         */
        math::Point2d principle_point_pitch() const;

        void set_principle_point_pitch( const math::Point2d& offset,
                                        bool                 rebuild );

        /**
         * Get the camera origin in the frame of reference of the camera model.  This method is often used to
         * find the origin of a ray emanating from the focal point of the camera through a pixel on the image
         * plane (e.g. for computing ray-ray intersection in a stereo vision algorithm).
         * - Generally the input pixel is only used for linescan cameras.
         */
        Result<math::Point3d> camera_origin( const math::Point2d& pixel = math::Point2d() ) const override;

        /**
         * Set the camera origin
        */
        void set_camera_origin( const math::Point3d& position );

        /**
         * Pose is a rotation which moves a vector in camera coordinates into world coordinates.
         * 
         * - The pinhole camera position does not vary by pixel so the input pixel is ignored.
         */
        Result<math::Quaternion> camera_pose( const math::Point2d& pix = math::Vector2d() ) const override;

        /**
         * Set the Camera pose
         */
        void set_camera_pose( const math::Quaternion& pose );
        
        /**
         * Set the Camera pose
         */
        void set_camera_pose( const math::Matrix<double,3,3>& pose );

        /**
         * Get the coordinate frames
        */
        void coordinate_frame( math::Vector3d& u_vec,
                               math::Vector3d& v_vec,
                               math::Vector3d& w_vec ) const;

        /**
         * Set the coordinate frame
         */
        void set_coordinate_frame( const math::Vector3d& u_vec,
                                   const math::Vector3d& v_vec,
                                   const math::Vector3d& w_vec );

        /**
         * Get the x axis
         */
        math::Vector3d coordinate_frame_x_direction() const;

        /**
         * Get the y axis
         */
        math::Vector3d coordinate_frame_y_direction() const;

        /**
         * Get the z axis
         */
        math::Vector3d coordinate_frame_z_direction() const;

        /**
         * Get the distortion model
        */
        Distortion_Base::ptr_t distortion() const;

        /**
         * Set the distortion model
        */
        void set_distortion( const Distortion_Base::ptr_t& distortion );

        void intrinsic_parameters( math::Point2d& focal_length_pitch,
                                   math::Point2d& principle_point_pitch ) const;
        
        void set_intrinsic_parameters( const math::Point2d& focal_length_pitch,
                                       const math::Point2d& principle_point_pitch );

        math::Point2d focal_length_pitch() const;

        void set_focal_length_pitch( const math::Point2d& f,
                                     bool                 rebuild );

        double pitch() const;

        void set_pitch( double pitch );

        math::Matrix<double,3,4> camera_matrix() const;
        
        Result<void> set_camera_matrix( const math::Matrix<double,3,4>& p );

        Result<void> rebuild_camera_matrix();

        Result<void> apply_transform( const math::Matrix_4x4& transform );

        Result<void> apply_transform( const math::Matrix_3x3& rotation,
                                      const math::Vector3d&   translation,
                                      double                  scale );

        Result<Camera_Model_Pinhole::ptr_t> scale_camera( double scale ) const;

        Camera_Model_Pinhole::ptr_t strip_lens_distortion() const;

        /**
         * Get the "type" string for the specified camera-model.  These must be unique amongst registered camera-model
         * types.
         */
        std::string type() const override;
        
        /**
         * Print a human-readable output of the camera configuration
         */
        std::string to_string( size_t offset ) const override;

    private:

        /// Origin of the camera in cartesian
        math::Point3d m_camera_origin;

        /// Focal Length
        math::Point2d  m_focal_length_pitch { { 0, 0 } };

        /// Principle Point
        math::Point2d   m_principle_point_pitch { { 0, 0 } };

        /// Axis Directions
        math::Vector3d m_x_axis { { 1, 0, 0 } };
        math::Vector3d m_y_axis { { 0, 1, 0 } };
        math::Vector3d m_z_axis { { 0, 0, 1 } };

        /// Pitch Parameters
        double m_pitch { 0 };

        // Distortion Values
        geo::cam::Distortion_Base::ptr_t m_distortion;

        /// Set the point-to-pixel check
        bool m_do_point_to_pixel_check { true };

        math::Matrix<double,3,4> m_camera_matrix;

        // Stored for easy access.
        math::Matrix<double,3,3> m_rotation;
        math::Matrix<double,3,3> m_intrinsics;
        math::Matrix<double,3,4> m_extrinsics;

        /// Cached values for pixel_to_vector
        math::Matrix<double,3,3> m_inv_camera_transform;

}; // End of Camera_Model_Pinhole class

} // End of tmns::geo::cam namespace