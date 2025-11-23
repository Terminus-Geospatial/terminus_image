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
 * @file    File_Collection_Utilities.cpp
 * @author  Marvin Smith
 * @date    8/26/2023
 */
#include "File_Collection_Utilities.hpp"

// TOML++
#include <toml++/toml.h>

// Terminus Libraries
#include <terminus/log/utility.hpp>

#include "TOML_Utilities.hpp"

namespace tmns::image::cx::file {

/****************************************/
/*          Parse and INI File          */
/****************************************/
Result<void> parse_toml_collection_file( const std::filesystem::path&         input_path,
                                         std::vector<std::filesystem::path>&  image_list,
                                         geo::cam::Camera_Model_Base::ptr_t&  global_intrinsics )
{
    // Check if the input file exists
    if( !std::filesystem::exists( input_path ) )
    {
        return outcome::fail( core::error::ErrorCode::FILE_NOT_FOUND,
                              "Input path does not exist: ",
                              input_path );
    }

    toml::table tbl;
    try
    {
        tbl = toml::parse_file( input_path.native() );
        
        // Check if the imagery node exists
        auto collection_node = tbl["collection"];
        if( !!collection_node["image_list"] )
        {
            // Grab the image list
            auto list_node = collection_node["image_list"];
            if( !!list_node )
            {
                // increment each element with visit()
                list_node.as_array()->for_each([&](auto&& el)
                {
                    std::string element = el.as_string()->get();
            
                    image_list.push_back( element );
                });
            }
        }

        if( !!collection_node["images"] )
        {
            int image_id;
            std::string pathname;

            toml::table image_list_node = *collection_node["images"].as_table();
            image_list_node.for_each([&]( const toml::key& key, 
                                          toml::table& image_node )
            {
                // Check if the key is a number
                try
                {
                    // Get the key
                    pathname = image_node["pathname"].as_string()->get();;
                    
                    image_list.push_back( pathname );
                    
                }
                catch ( std::exception& e )
                {

                }
            });
        }

        // Check for global intrinsics
        if( !!collection_node["intrinsic"] )
        {
            auto tbl = *collection_node["intrinsic"].as_table();
            auto intrinsic_res = load_intrinsics( tbl );
            if( !intrinsic_res.has_error() )
            {
                global_intrinsics = intrinsic_res.value();
            }
        }
    }
    catch (const toml::parse_error& err)
    {
        return outcome::fail( core::error::ErrorCode::PARSING_ERROR,
                              "Problem Parsing TOML File: ",
                              input_path, "What: ", err.what() );
    }

    return outcome::ok();
}

}