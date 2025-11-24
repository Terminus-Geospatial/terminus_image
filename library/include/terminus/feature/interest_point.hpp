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
 * @file    interest_point.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/Point.hpp>
#include <terminus/math/vector.hpp>

// C++ Libraries
#include <vector>

namespace tmns::feature {

/**
 * @brief Class for organizing feature/interest point data
*/
class interest_point
{
    public:

        /**
         * Default Constructor
         */
        interest_point() = default;

        /**
         * Constructor
        */
        interest_point( float px,
                        float py );

        
        /**
         * Constructor
        */
        interest_point( const math::Point2f& pixel_loc,
                        float                scale,
                        float                angle_rad,
                        float                response,
                        int                  octave,
                        int                  class_id );

        /**
         * Get the pixel location
         */
        math::Point2f& pixel_loc();

        /**
         * Get the pixel location (const)
         */
        math::Point2f const& pixel_loc() const;

        /**
         * Get the raster location
         */
        math::Point2i& raster_loc();

        /**
         * Get the raster location (const)
         */
        math::Point2i const& raster_loc() const;

        /**
         * Scale of the signal response
         */
        float scale() const;

        /**
         * Angle of the descriptor in radians
         */
        float angle_radians() const;

        /**
         * Signal response
         */
        float response() const;

        /**
         * Octave
         */
        int octave() const;

        /**
         * Class ID Value
         */
        int class_id() const;

        /**
         * Get the descriptor
         */
        math::VectorN<float>& descriptors();

        /**
         * Get the descriptor
         */
        math::VectorN<float> const& descriptors() const;

    private:

        /// Pixel Location
        math::Point2f m_pixel_loc { { 0, 0 } };

        /// Raster Coords
        math::Point2i m_raster_loc { { 0, 0 } };

        /// Scale
        float m_scale { 1 };

        /// Angle in radians
        float m_angle_radians { -1 };

        /// Response
        float m_response { 0 };

        /// Octave
        int m_octave { 0 };

        /// Class ID
        int m_class_id { -1 };

        /// Descriptor Vector
        math::VectorN<float> m_descriptor;

}; // End of interest_point class


/// Typedef for interest_point_List
typedef std::vector<interest_point> interest_point_List;

} // End of tmns::feature namespace