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
 * @file    distortion_null.cpp
 * @author  Marvin Smith
 * @date    11/10/2023
*/
#include <terminus/geography/camera/distortion_null.hpp>


namespace tmns::geo::cam {

/********************************************************/
/*          Convert to Distorted Coordinates            */
/********************************************************/
math::Point2d distortion_null::to_distorted( const camera_model_pinhole& /*camera_model*/,
                                             const math::Point2d&        pixel_coord ) const
{
    return pixel_coord;
}

/**********************************************************/
/*          Convert to Undistorted Coordinates            */
/**********************************************************/
math::Point2d distortion_null::to_undistorted( const camera_model_pinhole& /*camera_model*/,
                                               const math::Point2d&        pixel_coord ) const
{
    return pixel_coord;
}

/********************************************/
/*      Check if Fast Distort Exists        */
/********************************************/
bool distortion_null::has_fast_distort() const
{
    return true;
}

/**********************************************/
/*      Check if Fast Undistort Exists        */
/**********************************************/
bool distortion_null::has_fast_undistort() const
{
    return true;
}

/****************************************************/
/*          Get the distortion parameters           */
/****************************************************/
std::vector<double> distortion_null::distortion_parameters() const
{
    return std::vector<double>();
}

/****************************************************/
/*          Set the Distortion Parameters           */
/****************************************************/
void distortion_null::set_distortion_parameters( const std::vector<double>& /*params*/ )
{
    throw std::runtime_error("Not Implemented Yet");
}

/************************************************************/
/*          Get the number of distortion parameters         */
/************************************************************/
size_t distortion_null::num_dist_params() const
{
    return 0;
}

/************************************/
/*          Name the Model          */
/************************************/
std::string distortion_null::name() const
{
    return "distortion_null";
}

/************************************/
/*          Scale the Model         */
/************************************/
Result<void> distortion_null::scale( double /*scale*/ )
{
    // do nothing
    return outcome::ok();
}

/************************************/
/*          Print String            */
/************************************/
std::string distortion_null::to_log_string( size_t offset ) const
{
    std::stringstream sout;
    std::string gap( offset, ' ' );

    sout << gap << " - " << name() << std::endl;

    return sout.str();
}

} // End of tmns::geo::cam namespace