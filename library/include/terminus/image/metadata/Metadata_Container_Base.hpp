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
 * @file    Metadata_Container_Base.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

/// C++ Libraries
#include <memory>

/// Boost Libraries
#include <boost/property_tree/ptree.hpp>

/// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

namespace tmns::image::meta {

/**
 * Storage container for image metadata.  This is a wrapper around boost::property_tree that allows for 
 * more imagery-specific calls.  
*/
class Metadata_Container_Base
{
    public:

        /// Pointer Type
        using ptr_t = std::shared_ptr<Metadata_Container_Base>;

        /// Property Tree Type
        using tree_type = boost::property_tree::ptree;

        Metadata_Container_Base() = default;

        /**
         * Get the number of child nodes
         */
        virtual size_t number_child_nodes() const;

        /**
         * Add Metadata Key/Value Pair
         * 
         * @param key
         * @param value
         * @param overwrite_match
         */
        template <typename ValueT>
        Result<void> insert( const std::string& key,
                             const ValueT&      value,
                             bool               overwrite_match = true )
        {
            if( overwrite_match && m_tree.count( key ) > 0 )
            {
                return outcome::ok();
            }
            m_tree.put<ValueT>( key, value );
            return outcome::ok();
        }

        /**
         * Merge with another container
         */
        virtual Result<void> insert( const Metadata_Container_Base::ptr_t container,
                                     bool                                 overwrite_matches );

        /**
         * Get the requested key
         */
        template <typename ValueT>
        Result<ValueT> get( const std::string& key_name ) const
        {
            if( m_tree.count( key_name ) <= 0 )
            {
                return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                                      "Unable to find key '", key_name, "'" );
            }
            return outcome::ok<ValueT>( m_tree.get<ValueT>( key_name ) );
        }

        /**
         * Print contents to pretty string
         */
        virtual std::string to_log_string( size_t offset = 0 ) const;

    private:

        /// Underlying Container
        tree_type m_tree;

}; // End of Metadata_Container_Base class

} // End of tmns::image::meta namespace