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
 * @file    Block_Utilities.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/Size.hpp>


namespace tmns::image::ops::block {

/**
 * Compute a default block size to use for block image operations.
 * @param rows
 * @param cols
 * @param planes
 *
 * @returns Block size represented as Size object
*/
template<typename PixelT>
math::Size2i get_default_block_size( size_t rows,
                                     size_t cols,
                                     size_t planes = 1 )
{
    const size_t default_block_size_bytes = 2 * 1024 * 1024; // 2 megabytes

    // Compute rough block size
    size_t block_rows = default_block_size_bytes / ( planes * cols * sizeof(PixelT) );

    // If too small, move to 1
    if( block_rows < 1 ) block_rows = 1;

    // If too large, clamp
    else if( block_rows > rows ) block_rows = rows;
    
    return math::Size2i( { (int) cols,
                           (int) block_rows } );
}



}  // End of tmns::image::ops::block namespace