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
 * @file    Image_Resource_Memory_OpenCV.hpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#pragma once

// C++ Libraries
#include <memory>

// Terminus Libraries
#include "../../Image_Resource_Memory.hpp"

namespace tmns::image::io::ocv {

/**
 * OpenCV driver for loading images into memory
*/
class Image_Resource_Memory_OpenCV : public Image_Resource_Memory
{
    public:

        /// Parent Pointer Type
        typedef Image_Resource_Memory::ptr_t ParentPtrT;

        /**
         * Create a new resource and open the image
         *
         * Result will return failure if it's unable to open the image.
        */
        static Result<ParentPtrT> create( const std::filesystem::path& pathname );

}; // end of Read_Image_Resource_Memory_OpenCV Class

} // end of tmns::image::io::ocv namespace