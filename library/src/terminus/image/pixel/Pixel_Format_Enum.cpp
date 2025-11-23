/**
 * @file    Pixel_Format_Enum.cpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#include "Pixel_Format_Enum.hpp"

// Terminus Libraries
#include <terminus/outcome/macros.hpp>

namespace tmns::image {

/********************************************/
/*      Convert Enumeration to String       */
/********************************************/
std::string enum_to_string( Pixel_Format_Enum val )
{
    switch( val )
    {
        case Pixel_Format_Enum::SCALAR:
            return "SCALAR";
        case Pixel_Format_Enum::GRAY:
            return "GRAY";
        case Pixel_Format_Enum::GRAYA:
            return "GRAYA";
        case Pixel_Format_Enum::RGB:
            return "RGB";
        case Pixel_Format_Enum::RGBA:
            return "RGBA";
        case Pixel_Format_Enum::GENERIC_3_CHANNEL:
            return "GENERIC_3_CHANNEL";
        case Pixel_Format_Enum::GENERIC_4_CHANNEL:
            return "GENERIC_4_CHANNEL";
        case Pixel_Format_Enum::UNKNOWN:
            return "UNKNOWN";
        default:

            throw std::runtime_error( "Don't be lazy!" );
    }
}

/****************************************************************/
/*          Get the number of channels for the PixelType        */
/****************************************************************/
Result<int> num_channels( Pixel_Format_Enum value )
{
    switch( value )
    {
        case Pixel_Format_Enum::GRAY:
        case Pixel_Format_Enum::SCALAR:
            return outcome::ok<int>( 1 );

        case Pixel_Format_Enum::GRAYA:
            return outcome::ok<int>( 2 );

        case Pixel_Format_Enum::RGB:
        case Pixel_Format_Enum::GENERIC_3_CHANNEL:
            return outcome::ok<int>( 3 );

        case Pixel_Format_Enum::RGBA:
        case Pixel_Format_Enum::GENERIC_4_CHANNEL:
            return outcome::ok<int>( 3 );

        case Pixel_Format_Enum::UNKNOWN:
            return outcome::fail( core::error::ErrorCode::INVALID_PIXEL_TYPE );

        default:
            throw std::runtime_error( "Don't be lazy" );
    }
}

} // end of tmns::image namespace