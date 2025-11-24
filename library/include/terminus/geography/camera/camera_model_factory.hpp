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
 * @file    camera_model_Factory.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

/// C++ Libraries
#include <memory>

/// Terminus Libraries
#include <terminus/error.hpp>

/// Terminus Image Libraries
#include "camera_model_base.hpp"

namespace tmns::geo::cam {

class camera_model_factory
{
    public:

        /// Pointer Type
        using ptr = std::shared_ptr<camera_model_factory>;

        /**
         * Construct a Camera-Model instance from an image metadata container
         * specification.
         */
        static Result<camera_model_base::ptr_t> create_from_metadata( image::meta::Metadata_Container_Base::ptr_t container );


}; // End of camera_model_Factory class

} // End of tmns::geo::cam namespace