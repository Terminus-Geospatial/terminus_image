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
 * @file    Base_Blob.hpp
 * @author  Marvin Smith
 * @date    8/13/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

// C++ Libraries
#include <memory>

namespace tmns::image::ops::blob {

/**
 * @class Base_Blob
 * 
 * Base interface for a "blob."  A blob is a mass of pixels with 
 * some important characteristics.  Rather than representing as a 
 * raster, we represent a chains of pixels.  This helps with sparse
 * representations.
 * 
 * Note that blobs should be somewhat read thread-safe, as this is likely to get
 * constructed, then read-from using multiple threads.
*/
template <typename ImplT>
class Base_Blob
{
    public:

        /// Pointer-Type
        typedef typename std::shared_ptr<Base_Blob<ImplT>> ptr_t;

        /// Implementation Type (For derived types if they want it)
        typedef ImplT impl_type;

        /// Methods to access derived type
        ImplT      & impl()       { return static_cast<ImplT      &>(*this); }
        ImplT const& impl() const { return static_cast<ImplT const&>(*this); }

}; // End of Base_Blob class

} // End of tmns::image::ops::blob namespace