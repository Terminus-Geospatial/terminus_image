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
 * @file    Camera_Model_Adjusted.cpp
 * @author  Marvin Smith
 * @date    9/15/2023
 */
#include "Camera_Model_Adjusted.hpp"

namespace tmns::geo::cam {

/************************************/
/*          Constructor             */
/************************************/
Camera_Model_Adjusted::Camera_Model_Adjusted( Camera_Model_Base::ptr_t camera_model,
                                              const math::Vector3d&    translation,
                                              const math::Quaternion&  rotation,
                                              const math::Point2d&     pixel_offset,
                                              double                   scale )
    : m_camera_model( camera_model ),
      m_translation( translation ),
      m_rotation( rotation ),
      m_rotation_inverse( rotation.inverse() ),
      m_pixel_offset( pixel_offset ),
      m_scale( scale )
{
}

} // End of tmns::geo::cam namespace