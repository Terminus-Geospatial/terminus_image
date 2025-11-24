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
 * @file    Image_Resource_Memory.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

// Terminus Libraries
#include "../types/Image_Resource_Base.hpp"

// Boost Libraries
#include <boost/utility.hpp>

namespace tmns::image::io {

/**
 * Memory/Driver loading interface
*/
class Image_Resource_Memory : public Image_Resource_Base,
                                     boost::noncopyable
{
    public:

        typedef std::shared_ptr<Image_Resource_Memory> ptr_t;

}; // End of Image_Resource_Memory class

} // end of tmns::image::io namespace