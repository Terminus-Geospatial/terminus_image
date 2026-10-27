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
 * @file    convert.hpp
 * @author  Marvin Smith
 * @date    7/16/2023
*/
#pragma once

// C++ Standard Libraries
#include <limits>

// Terminus Libraries
#include <terminus/error.hpp>

// Terminus Image Libraries
#include <terminus/image/types/image_buffer.hpp>

namespace tmns::image {

/// Convert any integer into a float in the -1 to +1 range.
template <class SrcT, class DestT>
void channel_convert_int_to_float( SrcT* src, DestT* dest )
{
  *dest = DestT(*src) * ( DestT( 1.0 ) / static_cast<DestT>( std::numeric_limits<SrcT>::max() ) );
}

/**
 * @brief Convert a float in the range 0 to +1 to an integer type.
 *
 * @tparam SrcT  Source type (float or double)
 * @tparam DestT Destination type (int or uint)
 * @param  src   Source pointer
 * @param  dest  Destination pointer
 */
template <class SrcT, class DestT>
void channel_convert_float_to_int( SrcT* src, DestT* dest )
{
    if( *src > SrcT(1.0) ) *dest = std::numeric_limits<DestT>::max();
    else if( *src < SrcT(0.0) ) *dest = DestT(0);
    else *dest = DestT( static_cast<double>(*src) * static_cast<double>( std::numeric_limits<DestT>::max() ) );
}

/**
 * Convert pixel data from input buffer-type to output type
 *
 * This is a big method under the hood, so it's a separate file to keep things
 * from spiraling out of control.  All other methods in this file are supporting.
 *
 * @param dst Destination pixel container
 * @param src Source pixel data
 * @param rescale Flag if we need to scale imagery
*/
Result<void> convert( const Image_Buffer&  dst,
                      const Image_Buffer&  src,
                      bool                 rescale = false );

} // End of tmns::image namespace