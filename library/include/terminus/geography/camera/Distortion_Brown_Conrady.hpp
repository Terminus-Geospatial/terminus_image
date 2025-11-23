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
 * @file    Distortion_Brown_Conrady.hpp
 * @author Marvin Smith
 * @date 11/10/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/geography/camera/Distortion_Base.hpp>

namespace tmns::geo::cam {

/**
 * @class Distortion_Brown_Conrady
 * 
 * Implements the Brown-Conrady distortion model.
*/
class Distortion_Brown_Conrady : public Distortion_Base
{
    public:

        static constexpr size_t NUM_DISTORTION_PARAMS { 8 };

        /**
         * Default Constructor
         */
        Distortion_Brown_Conrady() = default;

        /**
         * Separated Parameters
         */
        Distortion_Brown_Conrady( const math::Point2d&       principle_point_pitch,
                                  const std::vector<double>& radial_distortion,
                                  const std::vector<double>& tangential_distortion,
                                  double                     tangential_distortion_angle_rad );
        
        /**
         * Convert from Distorted to Undistorted pixel coordinates
         */
        math::Point2d to_undistorted( const Camera_Model_Pinhole& camera_model,
                                      const math::Point2d&        pixel_coord ) const override;

        /**
         * Return true if the undistorted_coordinates() implementation does not use a solver.
         */
        bool has_fast_undistort() const override;

        /**
         * Get the distortion parameters
         */
        std::vector<double> distortion_parameters() const override;

        /**
         * Set the list of distortion parameters
        */
        void set_distortion_parameters( const std::vector<double>& params ) override;
        
        /**
         * Number of distortion parameters
         */
        size_t num_dist_params() const override;

        /**
         * Each derived model needs to have a string name.
         */
        std::string name() const override;
    
        /**
         * Used to scale distortion with image size
         */
        Result<void> scale( double scale ) override;

        /**
         * Print a log-friendly string
         */
        std::string to_log_string( size_t offset = 0 ) const override;

        /**
         * Clone the instance
        */
        Distortion_Base::ptr_t copy() const override;

    private:

        /// Principle Point
        math::Point2d m_principle_point { { 0, 0 } };

        /// Radial Distortion
        math::Vector3d m_radial_distortion { { 0, 0, 0 } };

        /// Centering Distortion
        math::Vector2d m_centering_distortion { { 0, 0 } };

        /// Centering Angle (Phi)
        double m_centering_angle_rad { 0 };

}; // End of Distortion_Brown_Conrady class

} // End of tmns::geo::cam namespace