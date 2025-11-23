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
 * @file    Distortion_Brown_Conrady.cpp
 * @author  Marvin Smith
 * @date    11/10/2023
*/
#include <terminus/geography/camera/Distortion_Brown_Conrady.hpp>

#include <terminus/geography/camera/Camera_Model_Pinhole.hpp>

namespace tmns::geo::cam {

/*********************************/
/*          Constructor          */
/*********************************/
Distortion_Brown_Conrady::Distortion_Brown_Conrady( const math::Point2d&       principle_point_pitch,
                                                    const std::vector<double>& radial_distortion,
                                                    const std::vector<double>& tangential_distortion,
                                                    double                     tangential_distortion_angle_rad )
    : m_principle_point( principle_point_pitch ),
      m_radial_distortion( radial_distortion ),
      m_centering_distortion( tangential_distortion ),
      m_centering_angle_rad( tangential_distortion_angle_rad )
{
}

/********************************************************/
/*          Convert to undistorted coordintates         */
/********************************************************/
math::Point2d Distortion_Brown_Conrady::to_undistorted( const Camera_Model_Pinhole& camera_model,
                                                        const math::Point2d&        pixel_coord ) const
{
    auto offset       = camera_model.principle_point_pitch();
    auto intermediate = pixel_coord - m_principle_point - offset;
    double r2         = intermediate.magnitude_sq();
    double radial     = 1 + m_radial_distortion[0] * r2 + 
                            m_radial_distortion[1] * r2 * r2 +
                            m_radial_distortion[2] * r2 * r2 * r2;
    
    double tangental   = m_centering_distortion[0] * r2 + m_centering_distortion[1] * r2 * r2;
    intermediate *= radial;
    intermediate[0] -= tangental * std::sin( m_centering_angle_rad );
    intermediate[1] += tangental * std::cos( m_centering_angle_rad );

    return intermediate+offset;
}

/********************************************/
/*      Show we have fast undistortion      */
/********************************************/
bool Distortion_Brown_Conrady::has_fast_undistort() const
{
    return true;
}

/****************************************************/
/*      Get the current distortion parameters       */
/****************************************************/
std::vector<double> Distortion_Brown_Conrady::distortion_parameters() const
{
    std::vector<double> output;
    std::copy( m_principle_point.begin(),
               m_principle_point.end(),
               output.begin() );
    

    std::copy( m_radial_distortion.begin(),
               m_radial_distortion.end(),
               output.end() );
    
    std::copy( m_centering_distortion.begin(),
               m_centering_distortion.end(),
               output.end() );
    
    output.push_back( m_centering_angle_rad );

    return output;
}

/****************************************************/
/*      Set the current distortion parameters       */
/****************************************************/
void Distortion_Brown_Conrady::set_distortion_parameters( const std::vector<double>& params )
{
    m_principle_point   = math::Point2d( { params[0],
                                           params[1] } );
    
    m_radial_distortion = math::Vector3d( { params[2], 
                                            params[3],
                                            params[4] } );
    
    m_centering_distortion = math::Vector2d( { params[5],
                                               params[6] } );

    m_centering_angle_rad = params[7];
}

/********************************************/
/*      Number of distortion parameters     */
/********************************************/
size_t Distortion_Brown_Conrady::num_dist_params() const
{
    return NUM_DISTORTION_PARAMS;
}

/********************************************/
/*      Get the Distortion class name       */
/********************************************/
std::string Distortion_Brown_Conrady::name() const
{
    return "Distortion_Brown_Conrady";
}

/********************************************/
/*          Scale Distortion Model          */
/********************************************/
Result<void> Distortion_Brown_Conrady::scale( double scale )
{
    return outcome::fail( core::error::ErrorCode::NOT_IMPLEMENTED,
                          "Not implemented for ", name() );
}

/****************************************************/
/*          Print to Log-Friendly String            */
/****************************************************/
std::string Distortion_Brown_Conrady::to_log_string( size_t offset ) const
{
    std::stringstream sout;
    std::string gap( offset, ' ' );

    sout << gap << " - " << name() << std::endl;
    sout << gap << "    - Principle Point: " << m_principle_point.to_log_string( offset );
    sout << gap << "    - Radial Distortion: " << m_radial_distortion.to_log_string( offset );
    sout << gap << "    - Tangential Distortion: " << m_centering_distortion.to_log_string( offset );
    sout << gap << "    - Phi Angle (radians): " << std::fixed << m_centering_angle_rad << std::endl;

    return sout.str();
}

/****************************************************/
/*          Create a clone of this instance         */
/****************************************************/
Distortion_Base::ptr_t Distortion_Brown_Conrady::copy() const
{
    Distortion_Base::ptr_t output;
    output = std::make_unique<Distortion_Brown_Conrady>( *this );

    return output;
}

} // End of tmns::geo::cam namespace