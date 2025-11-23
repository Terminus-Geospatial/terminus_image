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
 * @file    Distortion_Null.hpp
 * @author  Marvin Smith
 * @date    11/10/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/geography/camera/Distortion_Base.hpp>

namespace tmns::geo::cam {

/**
 * Lens distortion model that is mostly just a pass-through
 */
class Distortion_Null : public Distortion_Base
{
    public:

        /**
         * Destructor
         */
        ~Distortion_Null() override = default;

        /**
         * Convert from Undistorted to Distorted
         */
        math::Point2d to_distorted( const Camera_Model_Pinhole& camera_model,
                                    const math::Point2d&        pixel_coord ) const override;

        /**
         * Convert from Distorted to Undistorted
         */
        math::Point2d to_undistorted( const Camera_Model_Pinhole& camera_model,
                                      const math::Point2d&        pixel_coordd ) const override;

        /**
         * Check if method has fast distortion (true)
         */
        bool has_fast_distort() const override;

        /**
         * Check if method has fast undistortion (true)
         */
        bool has_fast_undistort() const override;

        /**
         * Return a vector containing all the distortion parameters.
         */
        std::vector<double> distortion_parameters() const override;

        /**
         * Initialize the object from a set of distortion parameters.
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
         * Print log-friendly string
        */
        std::string to_log_string( size_t offset ) const override;

}; // End of Distortion_Null class

} // End of tmns::geo::cam namespace