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
 * @file    Channel_Type_Enum.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

namespace tmns::image {

enum Channel_Type_Enum
{
    UNKNOWN     =  0 /**< Error condition.*/,
    UINT8       =  1,
    UINT12      =  2,
    UINT14      =  3,
    UINT16      =  4,
    UINT32      =  5,
    UINT64      =  6,
    INT8        =  7,
    INT16       =  8,
    INT32       =  9,
    INT64       = 10,
    FLOAT32     = 11,
    FLOAT64     = 12,
    FLOAT32Free = 13,
    FLOAT64Free = 14,
}; // end of Channel_Type_Enum enumeration

/**
 * Convert Channel_Type_Enum to a string
*/
std::string enum_to_string( Channel_Type_Enum val );

/**
 * Return true if enumeration is "integer" type (aka not float)
*/
bool is_integer_type( Channel_Type_Enum value );

/**
 * Function to get the size of the channel in bytes.
 * Throws an error if value is unknown.
*/
Result<size_t> channel_size_bytes( Channel_Type_Enum val );

} // end of tmns::image namespace