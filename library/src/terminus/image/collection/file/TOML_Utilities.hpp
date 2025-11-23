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
 * @file    TOML_Utilities.hpp
 * @author  Marvin Smith
 * @date    11/9/2023
*/
#pragma once 

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/geography/camera/Camera_Model_Base.hpp>

// Toml++
#include <toml++/toml.h>

namespace tmns::image::cx::file {

/**
 * Load the intrinsic parameters from the TOML node_view
 */
Result<geo::cam::Camera_Model_Base::ptr_t> load_intrinsics( toml::table view );

} // End of tmns::image::cx::file namespace