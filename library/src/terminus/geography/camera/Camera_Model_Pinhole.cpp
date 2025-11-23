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
 * @file    Camera_Model_Pinhole.cpp
 * @author  Marvin Smith
 * @date    11/9/2023
*/
#include <terminus/geography/camera/Camera_Model_Pinhole.hpp>

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/math/linalg/Operations.hpp>
#include <terminus/math/matrix.hpp>
#include <terminus/math/vector/Sub_Vector.hpp>

namespace tmns::geo::cam {

/************************************/
/*          Constructor             */
/************************************/
Camera_Model_Pinhole::Camera_Model_Pinhole( const math::Point3d&    camera_origin,
                                            const math::Point2d&    focal_length_pitch,
                                            const math::Point2d&    principle_point_pitch,
                                            const math::Vector3d&   x_axis_direction,
                                            const math::Vector3d&   y_axis_direction,
                                            const math::Vector3d&   z_axis_direction,
                                            double                  pitch,
                                            Distortion_Base::ptr_t  distortion )
    : m_camera_origin( camera_origin ),
      m_focal_length_pitch( focal_length_pitch ),
      m_principle_point_pitch( principle_point_pitch ),
      m_x_axis( x_axis_direction ),
      m_y_axis( y_axis_direction ),
      m_z_axis( z_axis_direction ),
      m_pitch( pitch )
{
    /*
    if( distortion )
    {
        m_distortion = m_distortion->copy();
    }
    else
    {
        m_distortion = std::make_unique<Distortion_Null>();
    }
    */
   throw std::runtime_error("Not implemented yet");

    rebuild_camera_matrix();
}

/******************************************/
/*      Convert 3d point to 2d Pixel      */
/******************************************/
math::Point2d Camera_Model_Pinhole::point_to_pixel_no_check( const math::Point3d& point ) const
{
    // Multiply the pixel location by the 3x4 camera matrix.
    // - The pixel coordinate is de-homogenized by dividing by the denominator.
    double denominator = m_camera_matrix( 2, 0 ) * point(0) + m_camera_matrix( 2, 1 ) * point(1) +
                         m_camera_matrix( 2, 2 ) * point(2) + m_camera_matrix( 2, 3 );
  
    math::Point2d pixel( { ( m_camera_matrix( 0, 0 ) * point( 0 ) + m_camera_matrix( 0, 1 ) * point( 1 ) +
                             m_camera_matrix( 0, 2 ) * point( 2 ) + m_camera_matrix( 0, 3 )             ) / denominator,
                           ( m_camera_matrix( 1, 0 ) * point( 0 ) + m_camera_matrix( 1, 1 ) * point( 1 ) +
                             m_camera_matrix( 1, 2 ) * point( 2 ) + m_camera_matrix( 1, 3 )             ) / denominator } );

    // Apply the lens distortion model
    // - Divide by pixel pitch to convert from metric units to pixels if the intrinsic
    //   values were not specified in pixel units (in that case m_pixel_pitch == 1.0)
    math::Point2d final_pixel = m_distortion->to_distorted( *this, pixel ) / m_pitch;

    return final_pixel;
}

/******************************************/
/*       Convert 3d point to 2d pixel      */
/******************************************/
Result<math::Point2d> Camera_Model_Pinhole::point_to_pixel( const math::Point3d& point ) const
{
    // Get the pixel using the no check version, then perform the check.
    math::Point2d final_pixel = point_to_pixel_no_check( point );
  
    if( !m_do_point_to_pixel_check )
    {
        return outcome::ok<math::Point2d>( final_pixel );
    }

    // Go back from the pixel to the vector and see how much difference there is.
    // - If there is too much error, the lens distortion model must have bugged out
    //   on this coordinate and it means we failed to project the point.
    // - Doing this slows things down but it is important to catch these failures.
    const double ERROR_THRESHOLD = 0.01;
    math::Point3d pixel_vector = pixel_to_vector( final_pixel ).value();
    math::Point3d phys_vector  = math::normalize( point - this->camera_origin().value() );
    double  diff = ( pixel_vector - phys_vector ).magnitude();

    // Print an explicit error message, otherwise this is confusing when showing up.
    if (diff >= ERROR_THRESHOLD )
    {
        std::stringstream sout;
        sout << "Failed to project point_to_pixel() accurately. Diff: " << diff;
        throw std::runtime_error( sout.str() );
    }
    return outcome::ok<math::Point2d>( final_pixel );
}

/*******************************************************************************/
/*      Option to turn off and on the point to pixel check (default is on).    */
/*******************************************************************************/
void Camera_Model_Pinhole::set_do_point_to_pixel_check( bool value )
{
    m_do_point_to_pixel_check = value;
}

/********************************************************************/
/*    Convert 3d point to 2d pixel without distortion correction    */
/********************************************************************/
math::Point2d Camera_Model_Pinhole::point_to_pixel_no_distortion( const math::Point3d& point ) const
{
    // Multiply the pixel location by the 3x4 camera matrix.
    // - The pixel coordinate is de-homogenized by dividing by the denominator.
    double denominator = m_camera_matrix( 2, 0 ) * point( 0 ) + m_camera_matrix( 2, 1 ) * point( 1 ) +
                         m_camera_matrix( 2, 2 ) * point( 2 ) + m_camera_matrix( 2, 3 );
    
    math::Point2d pixel( { ( m_camera_matrix( 0, 0 ) * point( 0 ) + m_camera_matrix( 0, 1 ) * point( 1 ) +
                             m_camera_matrix( 0, 2 ) * point( 2 ) + m_camera_matrix( 0, 3 ) ) / denominator,
                           ( m_camera_matrix( 1, 0 ) * point( 0 ) + m_camera_matrix( 1, 1 ) * point( 1 ) +
                             m_camera_matrix( 1, 2 ) * point( 2 ) + m_camera_matrix( 1, 3 ) ) / denominator } );

    // Divide by pixel pitch to convert from metric units to pixels if the intrinsic
    //   values were not specified in pixel units (in that case m_pixel_pitch == 1.0)
    return pixel / m_pitch;
}

/**************************************************/
/*      Convert Pixel Coordinate to 3D Vector     */
/**************************************************/
Result<math::Vector3d> Camera_Model_Pinhole::pixel_to_vector( const math::Point2d& pix ) const
{
    /*
    // Apply the inverse lens distortion model
    math::Point2d undistorted_pix = m_distortion->to_undistorted( *this, pix * m_pitch );

    // Compute the direction of the ray emanating from the camera center.
    math::Vector3d p( { 0, 0, 1 } );
    math::subvector( p, 0, 2 ) = undistorted_pix;
    return outcome::ok<math::Vector3d>( math::normalize( m_inv_camera_transform * p ) );
    */
    return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                         "Method not implemented yet" );
}

/********************************************/
/*      Test if the projection is valid     */
/********************************************/
bool Camera_Model_Pinhole::projection_valid( const math::Point3d& point ) const
{
    // z coordinate after extrinsic transformation
    double z = m_extrinsics( 2, 0 ) * point( 0 ) + m_extrinsics( 2, 1 ) * point( 1 ) +
               m_extrinsics( 2, 2 ) * point( 2 ) + m_extrinsics( 2, 3 );
    return ( z > 0 );
}

/********************************************/
/*          Get the Principle Point         */
/********************************************/
math::Point2d Camera_Model_Pinhole::principle_point_pitch() const
{
    return m_principle_point_pitch;
}

/********************************************/
/*          Set the Principle Point         */
/********************************************/
void Camera_Model_Pinhole::set_principle_point_pitch( const math::Point2d& point,
                                                      bool                 rebuild )
{
    m_principle_point_pitch = point;
    if (rebuild) rebuild_camera_matrix();
}

/******************************************/
/*    Get the Camera Center Coordinate    */
/******************************************/
Result<math::Point3d> Camera_Model_Pinhole::camera_origin( const math::Point2d& /*pix*/ ) const
{
    return m_camera_origin;
};

/****************************************/
/*    Set the camera center position    */
/****************************************/
void Camera_Model_Pinhole::set_camera_origin( const math::Point3d& position )
{
    m_camera_origin = position; 
    rebuild_camera_matrix();
}

/**********************************/
/*      Grab the camera pose      */
/**********************************/
Result<math::Quaternion> Camera_Model_Pinhole::camera_pose( const math::Point2d& /*pix*/ ) const
{
    return outcome::ok<math::Quaternion>( math::Quaternion::from_matrix( m_rotation ) );
}

/****************************/
/*    Set the camera pose   */
/****************************/
void Camera_Model_Pinhole::set_camera_pose( const math::Quaternion& pose )
{
    m_rotation = pose.to_rotation_matrix(); 
    rebuild_camera_matrix();
}

/**********************************/
/*      Set the camera matrix     */
/**********************************/
void Camera_Model_Pinhole::set_camera_pose( const math::Matrix<double,3,3>& pose)
{
    m_rotation = pose; 
    rebuild_camera_matrix();
}

/****************************************/
/*      Update the coordinate frame     */
/****************************************/
void Camera_Model_Pinhole::coordinate_frame( math::Vector3d& u_vec,
                                             math::Vector3d& v_vec,
                                             math::Vector3d& w_vec ) const
{
    u_vec = m_x_axis;
    v_vec = m_y_axis;
    w_vec = m_z_axis;
}

/**************************************/
/*      Set the coordinate frame      */
/**************************************/
void Camera_Model_Pinhole::set_coordinate_frame( const math::Vector3d& u_vec,
                                                 const math::Vector3d& v_vec,
                                                 const math::Vector3d& w_vec )
{
    m_x_axis = u_vec;
    m_y_axis = v_vec;
    m_z_axis = w_vec;

    rebuild_camera_matrix();
}

/****************************************/
/*      Get the X Coordinate Frame      */
/****************************************/
math::Vector3d Camera_Model_Pinhole::coordinate_frame_x_direction() const
{
    return m_x_axis;
}

/****************************************/
/*      Get the X Coordinate Frame      */
/****************************************/
math::Vector3d Camera_Model_Pinhole::coordinate_frame_y_direction() const
{
    return m_y_axis;
}

/****************************************/
/*      Get the Z Coordinate Frame      */
/****************************************/
math::Vector3d Camera_Model_Pinhole::coordinate_frame_z_direction() const
{
    return m_z_axis;
}

/******************************************/
/*      Get the lens distortion model     */
/******************************************/
Distortion_Base::ptr_t Camera_Model_Pinhole::distortion() const
{
    return m_distortion->copy();
}

/**************************************/
/*      Set the distortion model      */
/**************************************/
void Camera_Model_Pinhole::set_distortion( const Distortion_Base::ptr_t& distortion )
{
    m_distortion = distortion->copy();
}

/******************************************/
/*      Get the intrinsic parameters      */
/******************************************/
void Camera_Model_Pinhole::intrinsic_parameters( math::Point2d& focal_length_pitch,
                                                 math::Point2d& principle_point_pitch ) const
{
    focal_length_pitch    = m_focal_length_pitch;
    principle_point_pitch = m_principle_point_pitch;
}

/******************************************/
/*      Get the intrinsic parameters      */
/******************************************/
void Camera_Model_Pinhole::set_intrinsic_parameters( const math::Point2d& focal_length_pitch,
                                                     const math::Point2d& principle_point_pitch )
{
    m_focal_length_pitch    = focal_length_pitch;
    m_principle_point_pitch = principle_point_pitch;
    rebuild_camera_matrix();
}

/**********************************/
/*      Get the Focal Length      */
/**********************************/
math::Point2d Camera_Model_Pinhole::focal_length_pitch() const
{
    return m_focal_length_pitch;
}

/**********************************/
/*      Set the Focal Length      */
/**********************************/
void Camera_Model_Pinhole::set_focal_length_pitch( const math::Point2d& f,
                                                   bool                 rebuild )
{
    m_focal_length_pitch = f;
    if (rebuild)
    {
        rebuild_camera_matrix();
    }
}

/************************************/
/*          Get the Pitch           */
/************************************/
double Camera_Model_Pinhole::pitch() const
{
    return m_pitch;
}

/************************************/
/*          Set the Pitch           */
/************************************/
void Camera_Model_Pinhole::set_pitch( double pitch )
{
    m_pitch = pitch;
}

/**********************************/
/*      Set the Camera Matrix     */
/**********************************/
Result<void> Camera_Model_Pinhole::set_camera_matrix( const math::Matrix<double,3,4>& p )
{
    /*
    // Solving for camera center
    math::MatrixN<double> cam_nullsp = math::linalg::nullspace( p );
    auto cam_center = math::select_col( cam_nullsp, 0 );
    cam_center /= cam_center[3];
    m_camera_origin = math::subvector(cam_center,0,3);

    // Solving for intrinsics with RQ decomposition
    auto M = submatrix(p,0,0,3,3);
    math::MatrixN<double> R,Q;
    math::rqd( M, R, Q );
    math::Matrix<double> sign_fix(3,3);
    sign_fix.set_identity();
    if( R(0,0) < 0 )
    {
        sign_fix(0,0) = -1;
    }
    if( R(1,1) < 0 )
    {
        sign_fix(1,1) = -1;
    }
    if( R(2,2) < 0 )
    {
      sign_fix(2,2) = -1;
    }
    R = R * sign_fix;
    Q = sign_fix * Q;
    R /= R(2,2);

    // Pulling out intrinsic and last extrinsic
    math::Matrix<double,3,3> uvwRotation;
    math::select_row( uvwRotation, 0 ) = m_x_direction;
    math::select_row( uvwRotation, 1 ) = m_y_direction;
    math::select_row( uvwRotation, 2 ) = m_z_direction;
    math::m_rotation = math::inverse( uvwRotation * Q );
    m_fu = R(0,0);
    m_fv = R(1,1);
    m_cu = R(0,2);
    m_cv = R(1,2);

    if( std::fabs( R( 0, 1 ) ) >= 1.2 )
    {
        std::stringstream sout;
        sout << "Significant skew not modelled by pinhole camera";
        return outcome::fail( core::error::ErrorCode::UNDEFINED,
                              sout.str() );
    }
    
    // Rebuild
    return rebuild_camera_matrix();
    */
   return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                         "Method not implemented yet" );
}

/************************************/
/*      Get the camera matrix       */
/************************************/
math::Matrix<double,3,4> Camera_Model_Pinhole::camera_matrix() const
{
    return m_camera_matrix;
}

/************************************************/
/*      Reconstruct the Camera Model Matrix     */
/************************************************/
Result<void> Camera_Model_Pinhole::rebuild_camera_matrix()
{
    /*
    /// The intrinsic portion of the camera matrix is stored as
    ///
    ///    [  fx   0   cx  ]
    /// K= [  0    fy  cy  ]
    ///    [  0    0   1   ]
    ///
    /// with fx, fy the focal length of the system (in horizontal and
    /// vertical pixels), and (cx, cy) the pixel coordinates of the
    /// central pixel (the principal point on the image plane).

    m_intrinsics(0,0) = m_fu;
    m_intrinsics(0,1) = 0;
    m_intrinsics(0,2) = m_cu;
    m_intrinsics(1,0) = 0;
    m_intrinsics(1,1) = m_fv;
    m_intrinsics(1,2) = m_cv;
    m_intrinsics(2,0) = 0;
    m_intrinsics(2,1) = 0;
    m_intrinsics(2,2) = 1;

    // The extrinsics are normally built as the matrix:  [ R | -R*C ].
    // To allow for user-specified coordinate frames, the
    // extrinsics are now build to include the u,v,w rotation
    //
    //               | u_0  u_1  u_2  |
    //     Extr. =   | v_0  v_1  v_2  | * [ R | -R*C]
    //               | w_0  w_1  w_2  |
    //
    // The vectors u,v, and w must be orthonormal.

    //  check for orthonormality of u,v,w
    if( math::VectorNd::dot( m_x_axis, m_y_axis ) > 0.001 )
    {
        std::stringstream sout;
        sout << "UV is not orthonormal. Dot of u, v is not 0";
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              sout.str() );
    }
    if( math::VectorNd::dot( m_x_axis, m_z_axis ) > 0.001 )
    {
        std::stringstream sout;
        sout << "UW is not orthonormal. Dot of u, w is not 0";
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              sout.str() );
    }
    if( math::VectorNd::dot( m_y_axis, m_z_axis ) > 0.001 )
    {
        std::stringstream sout;
        sout << "VW is not orthonormal. Dot of v, w is not 0";
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              sout.str() );
    }
    if( std::fabs( m_x_axis.magnitude() - 1 ) > 0.001 )
    {
        std::stringstream sout;
        sout << "U is not unit-length";
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              sout.str() );
    }
    if( std::fabs( m_y_axis.magnitude() - 1 ) > 0.001 )
    {
        std::stringstream sout;
        sout << "V is not unit-length";
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              sout.str() );
    }
    if( std::fabs( m_z_axis.magnitude() - 1 ) < 0.001 )
    {
        std::stringstream sout;
        sout << "W is not unit-length";
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              sout.str() );
    }

    math::Matrix_3x3 uvwRotation;

    math::select_row( uvwRotation, 0 ) = m_x_axis;
    math::select_row( uvwRotation, 1 ) = m_y_axis;
    math::select_row( uvwRotation, 2 ) = m_z_axis;

    math::Matrix_3x3 rotation_inverse = math::transpose( m_rotation );
    math::submatrix(m_extrinsics,0,0,3,3) = uvwRotation * rotation_inverse;
    math::select_col( m_extrinsics, 3 ) = uvwRotation * ( -rotation_inverse ) * m_camera_center;

    m_camera_matrix = m_intrinsics * m_extrinsics;

    m_inv_camera_transform = math::inverse( uvwRotation * rotation_inverse )
                           * math::inverse( m_intrinsics );
    */
   return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                         "Method not implemented yet" );
}

/************************************************************************************/
/*    Apply a given rotation + translation + scale transform to a pinhole camera    */
/************************************************************************************/
Result<void> Camera_Model_Pinhole::apply_transform( const math::Matrix_4x4& transform )
{
    /*
    // Make sure homogenous
    if( transform(3, 3) != 1 )
    {
        std::stringstream sout;
        sout << "Expecting a similarity transform with value 1 in the lower-right corner.";
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              sout.str() );
    }

    // Extract the 3x3 component
    math::Matrix_3x3 R = math::submatrix( transform, 0, 0, 3, 3 );
    math::Vector3d T;
    for( size_t r = 0; r < 3; r++ )
    {
        T[r] = math::transpose( r, 3 );
    }
    
    double scale = std::pow( std::det(R), 1.0/3.0 );
    for( size_t r = 0; r < R.rows(); r++ )
    for( size_t c = 0; c < R.cols(); c++ )
    {
        R(r, c) /= scale;
    }

    this->apply_transform( R, T, scale );
    */
   return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                         "Method not implemented yet" );
}

/**********************************************************/
/*      Apply a given rotation + translation + scale      */
/*      transform to a pinhole camera                     */
/**********************************************************/
Result<void> Camera_Model_Pinhole::apply_transform( const math::Matrix_3x3& rotation,
                                                    const math::Vector3d&   translation,
                                                    double                  scale )
{
    /*
    // Extract current parameters
    math::Vector3d   position = this->camera_origin().value();
    math::Quaternion pose     = this->camera_pose().value();
  
    auto rotation_quaternion = Quaternion::from_matrix( rotation ).value();
  
    // New position and rotation
    position = scale * rotation * position + translation;
    pose     = rotation_quaternion * pose;
    this->set_camera_origin( position );
    this->set_camera_pose( pose );
    */
   return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                         "Method not implemented yet" );
}

/********************************************************************************/
/*      Scaling the camera is easy, just update the pixel pitch to account      */
/*      for the new image size.  This is not applying a scale transform to      */
/*      the camera, that is done in apply_transform().                          */
/********************************************************************************/
Result<Camera_Model_Pinhole::ptr_t> Camera_Model_Pinhole::scale_camera( double scale ) const
{
    /*
    if( scale == 0 )
    {
        return outcome::fail( core::error::ErrorCode::INVALID_INPUT,
                              "Camera_Model_Pinhole::scale_camera cannot have zero scale value!" );
    }
    auto focal  = focal_length_pitch();
    auto offset = principal_point_pitch();
    auto lens   = lens_distortion()->copy();

    auto new_camera = std::make_shared<Camera_Model_Pinhole>( camera_model.camera_center(),
                                                              focal,
                                                              offset,
                                                              coordinate_frame_x_direction(),
                                                              coordinate_frame_y_direction(),
                                                              coordinate_frame_z_direction(),
                                                              camera_pose().rotation_matrix(),
                                                              lens,
                                                              pitch / scale )
    return outcome::ok<Camera_Model_Pinhole::ptr>( new_camera );
    */
   return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED, 
                         "Method not implemented yet" );
}

/********************************************/
/*      Eject the lens distortion model     */
/********************************************/
Camera_Model_Pinhole::ptr_t Camera_Model_Pinhole::strip_lens_distortion() const
{
    /*
    return std::make_shared<Camera_Model_Pinhole>( camera_origin(),
                                                   camera_pose().rotation_matrix(),
                                                   focal_length_pitch(),
                                                   principle_point_pitch(),
                                                   coordinate_frame_x_direction(),
                                                   coordinate_frame_y_direction(),
                                                   coordinate_frame_z_direction(),
                                                   std::make_shared<Distortion_Null>(),
                                                   pixel_pitch() );
    */
   throw std::runtime_error( "Method not implemented yet" );
   return nullptr;
}

/****************************************/
/*          Get tye type string         */
/****************************************/
std::string Camera_Model_Pinhole::type() const
{
    return "Camera_Model_Pinhole";
}

/****************************************/
/*          Convert to String           */
/****************************************/
std::string Camera_Model_Pinhole::to_string( size_t offset ) const
{
    throw std::runtime_error("Not Implemented Yet");
    return "";
}

/*
// TODO: Verify this still works if we have used non-defaults for our 
//       directional axis vectors.
void epipolar(PinholeModel const &src_camera0, PinholeModel const &src_camera1,
              PinholeModel       &dst_camera0, PinholeModel       &dst_camera1) {

  typedef Matrix<double,3,3> RotMatrix;

  // Get input camera centers and rotations
  RotMatrix rot0    = src_camera0.camera_pose().rotation_matrix();
  RotMatrix rot1    = src_camera1.camera_pose().rotation_matrix();
  Vector3   center0 = src_camera0.camera_center();
  Vector3   center1 = src_camera1.camera_center();

  // The Z axis of the rotation matrix is the direction the camera is looking
  // - Multiply by negative one because the coordinate system used in the
  //   source of these equations is rotated 180 degrees along the X axis from ours.
  Vector3 look0 = -1*select_col(rot0,2);
  Vector3 look1 = -1*select_col(rot1,2);

  // Vector U is in the direction of one camera center to the next
  Vector3 u = (center1 - center0) / norm_2(center1 - center0);
  
  // W is going to be similar to the two look vectors but perpendicular to U.
  Vector3 mean_look = (look0+look1)/2.0;
  Vector3 temp      = cross_prod(u, cross_prod(mean_look, u));
  Vector3 w         = temp / norm_2(temp);
  
  // Vector V is dictated by the other two to form a coordinate frame.
  Vector3 v = cross_prod(w, u);

  // This is an alternate way to compute V and W, the results are similar.
  //Vector3 v     = cross_prod(look0, u);
  //Vector3 w     = cross_prod(u, v);
  
  // Account for the 180 degree coordinate system difference mentioned earlier by
  // negating the Y and Z columns.
  RotMatrix new_rot = RotMatrix(u[0], -v[0], -w[0],
                                u[1], -v[1], -w[1],
                                u[2], -v[2], -w[2]);    

  // The intrinsic values are somewhat arbitrary and are kept similar/identical to the input cameras.
  Vector2 focal_length = (src_camera0.focal_length() + src_camera1.focal_length()) / 2.0;
  Vector2 point_offset = (src_camera0.point_offset() + src_camera1.point_offset()) / 2.0;
  double  pixel_pitch  = (src_camera0.pixel_pitch () + src_camera1.pixel_pitch ()) / 2.0;

  // Set up the two output cameras.  Everything is the same except the positions.
  NullLensDistortion lens;
  dst_camera0 = PinholeModel(center0, new_rot,
                             focal_length [0], focal_length [1],
                             point_offset[0], point_offset[1],
                             &lens, pixel_pitch);
  dst_camera1 = PinholeModel(center1, new_rot,
                             focal_length [0], focal_length [1],
                             point_offset[0], point_offset[1],
                             &lens, pixel_pitch);
}
*/

} // End of tmns::geo::cam namespace