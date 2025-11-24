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
 * @file    camera_model_base.cpp
 * @author  Marvin Smith
 * @date    9/15/2023
 */
#include <terminus/geography/camera/camera_model_base.hpp>

namespace tmns::geo::cam {

/********************************/
/*          Get the pose        */
/********************************/
Result<math::Quaternion> camera_model_base::camera_pose( const math::Point2d& /*pixel*/ ) const
{
    return outcome::fail( error::Error_Code::NOT_IMPLEMENTED,
                          "Not implemented for camera-model-type: ",
                          this->type() );
}

} // End of tmns::geo::cam namespace