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
 * @file    Camera_Model_Adjusted.hpp
 * @author  Marvin Smith
 * @date    9/15/2023
*/
#pragma once

// Terminus Image Libraries
#include "Camera_Model_Base.hpp"

namespace tmns::geo::cam {

/**
 * Wraps an existing camera model with "tweakable" parameters.  This is helpful
 * during bundle-adjustment. he tweaks will be camera rotation around a fixed
 * center, camera translation, and in pixel space, pixel offset and pixel scale.
 * 
 * The adjusted camera is obtained by applying to the unadjusted camera the rigid
 * transform: 
 *       m_rotation*( P - m_rotation_center ) + m_rotation_center + m_translation
 * 
 * Here, P is in the world coordinate system. P is a point on the unadjusted camera,
 * and it becomes after the transform a point on the adjusted camera.
 */
class Camera_Model_Adjusted : public Camera_Model_Base
{
    public:

        /**
         * Constructor for the "Adjusted" camera model
         * 
         * @param camera_model Underlying camera model to use.
         * @param translation Initial starting translation (offset in 3d space)
         * @param rotation    Initial starting rotation (in 3d space)
         * @param pixel_offset Initial principle point offset (in 2d space)
         * @param scale         Pixel scale (initial) in 2d space
        */
        Camera_Model_Adjusted( Camera_Model_Base::ptr_t camera_model,
                               const math::Vector3d&    translation  = math::Vector3d(),
                               const math::Quaternion&  rotation     = math::Quaternion::from_matrix( math::Matrix_3x3::identity() ),
                               const math::Point2d&     pixel_offset = math::Point2d(),
                               double                   scale        = 1.0 );

    private:

        /// Underlying Camera Model
        Camera_Model_Base::ptr_t m_camera_model;

        /// Translation
        math::Vector3d m_translation { math::Vector3d( { 0, 0, 0 } ) };

        /// Rotation
        math::Quaternion m_rotation { math::Quaternion() };

        /// Inverse Rotation
        math::Quaternion m_rotation_inverse { math::Quaternion() };

        /// Pixel Offset
        math::Point2d m_pixel_offset { math::Point2d( { 0, 0 } ) };

        /// Scale
        double m_scale { 1.0 };

}; // End of Camera_Model_Adjusted class

} // End of tmns::geo::cam namespace