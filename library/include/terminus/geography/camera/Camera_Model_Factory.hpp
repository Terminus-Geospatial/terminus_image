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
 * @file    Camera_Model_Factory.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

/// C++ Libraries
#include <memory>

/// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

/// Terminus Image Libraries
#include "Camera_Model_Base.hpp"

namespace tmns::geo::cam {

class Camera_Model_Factory
{
    public:

        /// Pointer Type
        using ptr = std::shared_ptr<Camera_Model_Factory>;

        /**
         * Construct a Camera-Model instance from an image metadata container
         * specification.
         */
        static Result<Camera_Model_Base::ptr_t> create_from_metadata( image::meta::Metadata_Container_Base::ptr_t container );


}; // End of Camera_Model_Factory class

} // End of tmns::geo::cam namespace