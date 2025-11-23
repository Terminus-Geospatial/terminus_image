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

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

// Terminus Image Libraries
#include "../types/Image_Buffer.hpp"

namespace tmns::image {

/// Convert any integer into a float in the -1 to +1 range.
template <class SrcT, class DestT>
void channel_convert_int_to_float( SrcT* src, DestT* dest )
{
  *dest = DestT(*src) * ( DestT( 1.0 ) / static_cast<DestT>( std::numeric_limits<SrcT>::max() ) );
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