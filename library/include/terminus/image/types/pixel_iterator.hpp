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
 * @file    Pixel_Iterator.hpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#pragma once

// Boost Libraries
#include <boost/iterator/iterator_facade.hpp>

namespace tmns::image {

/**
 * @brief STL-compliant iterator for accessing regions of an image.
 *
 * This is heavily borrowed and inspired by NASA vision workbench.
 *
 * @link https://github.com/visionworkbench/visionworkbench/blob/master/src/vw/Image/PixelIterator.h
 *
 * Sequence of access are first column-by-column, then row-by-row, then channel-by-channel.
 * This allows use in most, if not all useful STL-compatible algorithms.  Using VW's experience,
 * we lift iterator_facade from boost to enable all of the various types.
 *
 * This should be used sparingly.  To iterate over an image takes a bit of practice and finese.
 * Jumping a channel or a row requires doing a lot more math than expected.  Consider using
 * block operations rather than STL algorithms.
 *
*/
template <typename ImageT>
class Pixel_Iterator : public boost::iterator_facade<Pixel_Iterator<ImageT>,
                                                     typename ImageT::pixel_type,
                                                     boost::random_access_traversal_tag,
                                                     typename ImageT::result_type,
                                                     int64_t>
{
    public:


    private:

}; // End of Pixel_Iterator class

} // End of tmns::image namespace