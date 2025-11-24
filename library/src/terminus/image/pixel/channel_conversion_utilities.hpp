/**
 * @file    Channel_Conversion_Utilities.hpp
 * @author  Marvin Smith
 * @date    7/16/2023
*/
#pragma once

namespace tmns::image {

/**
 * Determine the best type to accumulate values for the input type.
 * Helps prevent overflow and other issues caused by a lack of headroom.
*/
template <class T> struct Accumulator_Type {};
template <> struct Accumulator_Type<bool>      { typedef int      type; };
template <> struct Accumulator_Type<uint8_t>   { typedef int32_t  type; };
template <> struct Accumulator_Type<int8_t>    { typedef int32_t  type; };
template <> struct Accumulator_Type<uint16_t>  { typedef int32_t  type; };
template <> struct Accumulator_Type<int16_t>   { typedef int32_t  type; };
template <> struct Accumulator_Type<uint32_t>  { typedef int64_t  type; };
template <> struct Accumulator_Type<int32_t>   { typedef int64_t  type; };
template <> struct Accumulator_Type<uint64_t>  { typedef int64_t  type; };
template <> struct Accumulator_Type<int64_t>   { typedef int64_t  type; };
template <> struct Accumulator_Type<float>     { typedef double   type; };
template <> struct Accumulator_Type<double>    { typedef double   type; };


} // End of tmns::image namespace