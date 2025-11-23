/**
 * @file    Channel_Type_Enum.cpp
 * @author  Marvin Smith
 * @file    7/13/2023
*/
#include "Channel_Type_Enum.hpp"

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/outcome/Result.hpp>

namespace tmns::image {

/****************************************************/
/*          Convert Enumeration to String           */
/****************************************************/
std::string enum_to_string( Channel_Type_Enum val )
{
    switch( val )
    {
        case Channel_Type_Enum::UINT8:
            return "UINT8";
        case Channel_Type_Enum::UINT12:
            return "UINT12";
        case Channel_Type_Enum::UINT14:
            return "UINT14";
        case Channel_Type_Enum::UINT16:
            return "UINT16";
        case Channel_Type_Enum::UINT32:
            return "UINT32";
        case Channel_Type_Enum::INT8:
            return "INT8";
        case Channel_Type_Enum::INT16:
            return "INT16";
        case Channel_Type_Enum::INT32:
            return "INT32";
        case Channel_Type_Enum::FLOAT32:
            return "FLOAT32";
        case Channel_Type_Enum::FLOAT32Free:
            return "FLOAT32Free";
        case Channel_Type_Enum::FLOAT64:
            return "FLOAT64";
        case Channel_Type_Enum::FLOAT64Free:
            return "FLOAT64Free";
        case Channel_Type_Enum::UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

/************************************/
/*      Check if "integer" type     */
/************************************/
bool is_integer_type( Channel_Type_Enum val )
{
    switch( val )
    {
        case Channel_Type_Enum::UINT8:
        case Channel_Type_Enum::UINT12:
        case Channel_Type_Enum::UINT14:
        case Channel_Type_Enum::UINT16:
        case Channel_Type_Enum::UINT32:
        case Channel_Type_Enum::UINT64:
        case Channel_Type_Enum::INT8:
        case Channel_Type_Enum::INT16:
        case Channel_Type_Enum::INT32:
        case Channel_Type_Enum::INT64:
            return true;


        case Channel_Type_Enum::FLOAT32:
        case Channel_Type_Enum::FLOAT64:
        case Channel_Type_Enum::FLOAT32Free:
        case Channel_Type_Enum::FLOAT64Free:
        default:
            return false;
    }
}

/********************************************************/
/*          Get the size of the channel in bytes        */
/********************************************************/
Result<size_t> channel_size_bytes( Channel_Type_Enum val )
{
    switch( val )
    {
        // Single-Byte Entries
        case Channel_Type_Enum::UINT8:
        case Channel_Type_Enum::INT8:
            return outcome::ok<size_t>( 1 );

        // Two-byte entries
        case Channel_Type_Enum::UINT12:
        case Channel_Type_Enum::UINT14:
        case Channel_Type_Enum::UINT16:
        case Channel_Type_Enum::INT16:
            return outcome::ok<size_t>( 2 );

        // Four-byte entries
        case Channel_Type_Enum::UINT32:
        case Channel_Type_Enum::INT32:
        case Channel_Type_Enum::FLOAT32:
        case Channel_Type_Enum::FLOAT32Free:
            return outcome::ok<size_t>( 4 );

        // Eight-byte entries
        case Channel_Type_Enum::FLOAT64:
        case Channel_Type_Enum::FLOAT64Free:
            return outcome::ok<size_t>( 8 );

        // Error Conditions
        case Channel_Type_Enum::UNKNOWN:
        default:
            return outcome::fail( core::error::ErrorCode::INVALID_CHANNEL_TYPE );
    }
}

} // end of tmns::image namespace