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
 * @file    ISIS_JSON_Parser.cpp
 * @author  Marvin Smith
 * @date    9/30/2023
 */
#include "ISIS_JSON_Parser.hpp"

namespace tmns::image::io::gdal {

/****************************************/
/*          Parse a JSON String         */
/****************************************/
Result<meta::Metadata_Container_Base::ptr_t> ISIS_JSON_Parser::parse( const std::string& json_string )
{
    auto container = std::make_shared<meta::Metadata_Container_Base>();
    const std::string base_key = "isis";

    // Create a JSON string parser
    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse( json_string );

        for( auto& [key, value] : json.items() )
        {
            recursive_parse( container, 
                             value,
                             base_key + "." + key );
        }
    }
    catch( std::exception& e )
    {
        return outcome::fail( core::error::ErrorCode::PARSING_ERROR,
                              "Unable to parse JSON data. ", e.what() );
    }

    
    return outcome::ok<meta::Metadata_Container_Base::ptr_t>( container );
}

/************************************************/
/*          Recursively Parse JSON Object       */
/************************************************/
void ISIS_JSON_Parser::recursive_parse( meta::Metadata_Container_Base::ptr_t container,
                                        nlohmann::json&                      node,
                                        const std::string                    base_key )
{
    // Exit conditions
    if( node.empty() )
    {
        std::cerr << "Why is this node (" << base_key << ") empty?" << std::endl;
        std::abort();
    }
    else if( node.size() == 1 )
    {
        container->insert( base_key, node.front() );
    }

    // Keep digging deeper
    else
    {
        for( auto& [key, value] : node.items() )
        {
            recursive_parse( container,
                             value,
                             base_key + "." + key );
        }
    }
}

} // End of tmns::image::io::gdal namespace