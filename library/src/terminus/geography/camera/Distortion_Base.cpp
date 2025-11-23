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
 * @file    Distortion.cpp
 * @author  Marvin Smith
 * @date    11/10/2023
 */
#include <terminus/geography/camera/Distortion_Base.hpp>

// Terminus Libraries
#include <terminus/geography/camera/Camera_Model_Pinhole.hpp>
#include <terminus/math/optimization/Levenburg_Marquardt.hpp>


namespace tmns::geo::cam {

/**
 * Simple functor for optimizing the "to-distortion" operation
*/
struct Distort_Optimize_Functor :  public math::optimize::Least_Squares_Model_Base_Fixed<Distort_Optimize_Functor, 2, 2>
{
    typedef math::Vector2d result_type;
    typedef math::Vector2d domain_type;
    typedef math::Matrix<double, 2, 2> jacobian_type;

    const Camera_Model_Pinhole&  m_camera;
    const Distortion_Base&       m_distortion;
    
    /**
     * Constructor
     */
    Distort_Optimize_Functor( const Camera_Model_Pinhole& camera,
                              const Distortion_Base&      distortion )
        : m_camera( camera ),
          m_distortion( distortion )
    {
    }                              

    /**
     * Function Operator
    */
    result_type operator()( const domain_type& x ) const
    {
        return m_distortion.to_undistorted( m_camera, x );
    }
};

/**
 * Optimization functor for computing the undistorted coordinates using levenberg marquardt.
 */
struct Undistort_Optimize_Functor : public math::optimize::Least_Squares_Model_Base_Fixed<Undistort_Optimize_Functor, 2, 2>
{
    typedef math::Vector2d result_type;
    typedef math::Vector2d domain_type;
    typedef math::Matrix<double, 2, 2> jacobian_type;

    const Camera_Model_Pinhole&  m_camera;
    const Distortion_Base&       m_distortion;

    /**
     * Constructor
    */
    Undistort_Optimize_Functor( const Camera_Model_Pinhole& camera,
                                const Distortion_Base&      distortion )
        : m_camera( camera ),
          m_distortion( distortion )
    {
    }

    /**
     * Function Operator
    */
    result_type operator()( const domain_type& x ) const
    {
        return m_distortion.to_distorted( m_camera, x );
    }
};

/****************************************************************/
/*          Convert from Undistorted to Distorted Coords        */
/****************************************************************/
math::Point2d Distortion_Base::to_distorted( const Camera_Model_Pinhole& camera_model,
                                             const math::Point2d&        pixel_coord ) const
{
    Distort_Optimize_Functor model( camera_model, *this );
    math::optimize::LM_STATUS_CODE status;

    // Must push the solver really hard, to make sure bundle adjust gets accurate values to play with.
    auto solution = math::optimize::levenberg_marquardt_fixed( model,
                                                               pixel_coord,
                                                               pixel_coord,
                                                               status,
                                                               1e-16,
                                                               1e-16,
                                                               100 );
    
    if( status == math::optimize::LM_STATUS_CODE::ERROR_CONVERGED_REL_TOLERANCE )
    {
        tmns::log::error( ADD_CURRENT_LOC(), "Failed to converge." );
    }
  
    // Check if it failed badly to converge. That it did not converge is not on its own
    // unreasonable, sometimes the inputs are bad. But then the user must know about it.
    auto undist = this->to_undistorted( camera_model, solution.value() );
    double err = (undist - pixel_coord).magnitude() / std::max( pixel_coord.magnitude(), 0.1 ); // don't make this way too strict
    double tol = 1e-10;
    if (err > tol)
        throw std::runtime_error( "LensDistortion: Did not converge." );

  return solution.value();
}

/****************************************************************/
/*          Convert from Distorted to Undistorted Coords        */
/****************************************************************/
math::Point2d Distortion_Base::to_undistorted( const Camera_Model_Pinhole& camera_model,
                                               const math::Point2d&        pixel_coord ) const
{
    Undistort_Optimize_Functor model( camera_model, *this );
    math::optimize::LM_STATUS_CODE status;

    // Must push the solver really hard, to make sure bundle adjust gets accurate values
    // to play with.
    auto solution = math::optimize::levenberg_marquardt_fixed( model,
                                                               pixel_coord,
                                                               pixel_coord,
                                                               status,
                                                               1e-16,
                                                               1e-16,
                                                               100 );

    auto dist = this->to_distorted( camera_model, solution.value() );
    double err = (dist - pixel_coord).magnitude() / std::max( pixel_coord.magnitude(), 0.1); // don't make this way too strict
    double tol = 1e-10;
    if (err > tol)
        throw std::runtime_error( "LensDistortion: Did not converge." );
  
    //VW_ASSERT((status == math::optimization::eConvergedAbsTolerance), 
    //                 PixelToRayErr() << "distorted_coordinates: failed to converge." );
    //double error = norm_2(model(solution) - v);
    //std::cout << "status = " << status << ", input = " << v 
    //          << ", pixel = " << solution << ", error = " << error << std::endl;
    return solution.value();
}

/********************************************/
/*      Check if we have fast distortion    */
/********************************************/
bool Distortion_Base::has_fast_distort() const
{
    return false;
}

/**********************************************/
/*      Check if we have fast undistortion    */
/**********************************************/
bool Distortion_Base::has_fast_undistort() const
{
    return false;
}

/*******************************************/
/*      Get Distortion Parameters (none)   */
/*******************************************/
std::vector<double> Distortion_Base::distortion_parameters() const
{
    return std::vector<double>();
}

/*******************************************/
/*      Get Distortion Parameters (none)   */
/*******************************************/
void Distortion_Base::set_distortion_parameters( const std::vector<double>& params )
{
    // no-op
}

} // End of tmns::geo::cam namespace