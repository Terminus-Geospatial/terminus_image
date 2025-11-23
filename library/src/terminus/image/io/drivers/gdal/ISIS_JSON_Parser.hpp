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
 * @file    ISIS_JSON_Parser.hpp
 * @author  Marvin Smith
 * @date    9/28/2023
*/
#pragma once

// JSON Parsing Libraries
#include <nlohmann/json.hpp>

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

// Terminus Image Libraries
#include "../../../metadata/Metadata_Container_Base.hpp"

namespace tmns::image::io::gdal {

class ISIS_JSON_Parser
{
    public:

        /**
         * Parse a JSON string and return a JSON structure capable of decomposing the relevant ISIS data.
         */
        static Result<meta::Metadata_Container_Base::ptr_t> parse( const std::string& json_string );

        static void recursive_parse( meta::Metadata_Container_Base::ptr_t container,
                                     nlohmann::json&                      node,
                                     const std::string                    base_key );

}; // End of ISIS_JSON_Parser class

} // End of tmns::image::io::gdal namespace