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
 * @file    interest_point.cpp
 * @author  Marvin Smith
 * @date    8/11/2023
*/
#include "interest_point.hpp"

// Terminus Libraries
#include <terminus/math/Point_Utilities.hpp>

namespace tmns::feature {

/********************************/
/*          Constructor         */
/********************************/
interest_point::interest_point( float px,
                                float py )
    : m_pixel_loc( { px, py } ),
      m_raster_loc( { (int) std::round( px ),
                      (int) std::round( py ) } )
{
}

/********************************/
/*          Constructor         */
/********************************/
interest_point::interest_point( const math::Point2f& pixel_loc,
                                float                scale,
                                float                angle_radians,
                                float                response,
                                int                  octave,
                                int                  class_id )
    : m_pixel_loc( pixel_loc ),
      m_raster_loc( math::ToPoint2<int>( (int) std::round( pixel_loc.x() ),
                                         (int) std::round( pixel_loc.y() ) ) ),
      m_scale( scale ),
      m_angle_radians( angle_radians ),
      m_response( response ),
      m_octave( octave ),
      m_class_id( class_id )
{
}

/************************************/
/*      Get the Interest Point      */
/************************************/
math::Point2f& interest_point::pixel_loc()
{
    return m_pixel_loc;
}

/************************************/
/*      Get the Interest Point      */
/************************************/
math::Point2f const& interest_point::pixel_loc() const
{
    return m_pixel_loc;
}

/************************************/
/*      Get the Interest Point      */
/************************************/
math::Point2i& interest_point::raster_loc()
{
    return m_raster_loc;
}

/************************************/
/*      Get the Interest Point      */
/************************************/
math::Point2i const& interest_point::raster_loc() const
{
    return m_raster_loc;
}

/******************/
/*      Scale     */
/******************/
float interest_point::scale() const
{
    return m_scale;
}

/******************************/
/*      Angle in Radians      */
/******************************/
float interest_point::angle_radians() const
{
    return m_angle_radians;
}

/******************************/
/*          Response          */
/******************************/
float interest_point::response() const
{
    return m_response;
}

/**********************/
/*      Octave        */
/**********************/
int interest_point::octave() const
{
    return m_octave;
}

/**********************/
/*      Class ID      */
/**********************/
int interest_point::class_id() const
{
    return m_class_id;
}

/************************************/
/*      Get Feature Descriptor      */
/************************************/
math::VectorN<float>& interest_point::descriptors()
{
  return m_descriptor;
}

/************************************/
/*      Get Feature Descriptor      */
/************************************/
math::VectorN<float> const& interest_point::descriptors() const
{
  return m_descriptor;
}

} // End of tmns::feature namespace