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
 * @file    Image_Traits.hpp
 * @author  Marvin Smith
 * @date    7/21/2023
*/
#pragma once

// Terminus Libraries
#include "Image_Base.hpp"

// C++ Libraries
#include <type_traits>

namespace tmns::image {

/// Indicates whether a view can be resized via <B>set_size()</B>.
template <class ImplT>
struct Is_Resizable
{
    typedef std::false_type value;
};

/// Indicates whether a view type can be accessed at floating-point positions.
template <class ImplT>
struct Is_Floating_Point_Indexable
{
    static consteval bool value(){ return false; }
};

/// Indicates whether or not a type is an image base type.
template <class ImageT>
struct Is_Image_Base
{
    typedef typename std::is_base_of<Image_Base<ImageT>,ImageT>::type value;
};

/// Indicates whether or not a view can be accessed multiple times
/// just as efficiently as a locally-cached version.
template <class ImplT>
struct Is_Multiply_Accessible
{
    typedef std::false_type value;
};

} // End of tmns::image namespace