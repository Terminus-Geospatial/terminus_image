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
 * @file    Metadata_Container_Base.cpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#include <terminus/image/metadata/metadata_container_base.hpp>

// Terminus Libraries
#include <terminus/core/thirdparty/boost/ptree_utilities.hpp>

namespace tmns::image::meta {

/****************************************************/
/*          Get the number of child nodes           */
/****************************************************/
size_t Metadata_Container_Base::number_child_nodes() const
{
    return core::thirdparty::boost::number_child_nodes( m_tree );
}

/****************************************/
/*          Merge Containers            */
/****************************************/
Result<void> Metadata_Container_Base::insert( const Metadata_Container_Base::ptr_t container,
                                              bool                                 overwrite_matches )
{
    if( !container )
    {
        return outcome::fail( error::Error_Code::UNINITIALIZED,
                              "Input container is null." );
    }

    return core::thirdparty::boost::merge_ptrees( m_tree,
                                                  container->m_tree,
                                                  overwrite_matches );

}

/********************************************/
/*          Print to Pretty String          */
/********************************************/
std::string Metadata_Container_Base::to_log_string( size_t offset ) const
{
    std::stringstream sout;

    sout << core::thirdparty::boost::print_ptree( m_tree,
                                                  static_cast<int>(offset) );

    return sout.str();
}

} // End of tmns::image::meta