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
 * @file    Distortion_Base.hpp
 * @author  Marvin Smith
 * @date    11/9/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/math/Point.hpp>

// C++ Libraries
#include <memory>

namespace tmns::geo::cam {

// Pre-define the Camera model class
class Camera_Model_Pinhole;

class Distortion_Base
{
    public:

        // Underlying Pointer Type
        using ptr_t = std::unique_ptr<Distortion_Base>;

        /**
         * Destructor
        */
        virtual ~Distortion_Base() = default;

        virtual math::Point2d to_distorted( const Camera_Model_Pinhole& camera_model,
                                            const math::Point2d&        pixel_coord ) const;

        virtual math::Point2d to_undistorted( const Camera_Model_Pinhole& camera_model,
                                              const math::Point2d&        pixel_coord ) const;

        /**
         * Return true if the to_distorted() implementation does not use a solver.
         */
        virtual bool has_fast_distort() const;
    
        /**
         * Return true if the undistorted_coordinates() implementation does not use a solver.
         */
        virtual bool has_fast_undistort() const;

        /**
         * Return a vector containing all the distortion parameters.
         */
        virtual std::vector<double> distortion_parameters() const;

        /**
         * Initialize the object from a set of distortion parameters.
         */
        virtual void set_distortion_parameters( const std::vector<double>& params );

        /**
         * Number of distortion parameters
         */
        virtual size_t num_dist_params() const = 0;

        /**
         * Each derived model needs to have a string name.
         */
        virtual std::string name() const = 0;
    
        /**
         * Used to scale distortion with image size
         */
        virtual Result<void> scale( double scale ) = 0;
    
        /// Used to scale distortion with image size
        std::vector<std::string> distortion_param_names() const { return m_distortion_param_names; }

        /**
         * Print as log-friendly string
         */
        virtual std::string to_log_string( size_t offset ) const = 0;

        /**
         * Make a clone of the instance
        */
        virtual Distortion_Base::ptr_t copy() const = 0;

    protected:

        std::vector<std::string> m_distortion_param_names;

}; // End of Distortion_Base class

} // End of tmns::geo::cam namespace