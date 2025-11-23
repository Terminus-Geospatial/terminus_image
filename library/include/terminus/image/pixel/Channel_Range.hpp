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
 * @file    Channel_Range.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// C++ Libraries
#include <complex>

// Terminus Libraries
#include <terminus/math/types/Compound_Types.hpp>


namespace tmns::image {

/**
 * Wrapper class around logic determining the min/max values for imagery with pixels
 * of a certain channel-type.
 *
 * Instance for integer types
*/
template <class T, bool>
struct Channel_Range_Helper
{
    static T max()
    {
        return std::numeric_limits<T>::max();
    }
    static T min()
    {
        return T();
    }
};

/**
 * Wrapper class around logic determining the min/max values for imagery with pixels
 * of a certain channel-type.
 *
 * Instance for floating-point types
*/
template <class T>
struct Channel_Range_Helper<T,false>
{
    static T max()
    {
        return (T)(1.0);
    }
    static inline T min()
    {
        return (T)(0.0);
    }
};

/**
 * Wrapper class around logic determining the min/max values for imagery with pixels
 * of a certain channel-type.
 *
 * Instance for complex types
*/
template <typename T>
struct Channel_Range_Helper<std::complex<T>,false>
        : public Channel_Range_Helper<T,std::is_integral<T>::value> {};

/**
 * Class for computing the range of a channel
*/
template <typename T>
struct Channel_Range : public Channel_Range_Helper<typename math::Compound_Channel_Type<T>::type,
                                                   std::is_integral<typename math::Compound_Channel_Type<T>::type>::value> {};

} // End of tmns::image namespace