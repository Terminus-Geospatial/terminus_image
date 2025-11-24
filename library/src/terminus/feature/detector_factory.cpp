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
 * @file    detector_Factory.cpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#include <terminus/feature/detector_Factory.hpp>

namespace tmns::feature {

/********************************************************/
/*              Create a new feature detector           */
/********************************************************/
Result<detector_Base::ptr_t> detector_Factory::create_detector( detector_Config_Base::ptr_t config ) const
{
    // check null
    if( !config )
    {
        return outcome::fail( error::Error_Code::UNINITIALIZED,
                              "Configuration must be initialized.  Cannot be null." );
    }

    // Iterate over each generator
    for( const auto& generator : m_generators )
    {
        auto res = generator->generate( config );
        if( !res.has_error() )
        {
            return outcome::ok<detector_Base::ptr_t>( res.value() );
        }

    }
    return outcome::fail( error::Error_Code::NOT_FOUND,
                          "No driver found for config instance." );
}


/********************************************************/
/*              Create a new feature detector           */
/********************************************************/
Result<detector_Base::ptr_t> detector_Factory::create_extractor( detector_Config_Base::ptr_t config ) const
{
    // check null
    if( !config )
    {
        return outcome::fail( error::Error_Code::UNINITIALIZED,
                              "Configuration must be initialized.  Cannot be null." );
    }

    // Iterate over each generator
    for( const auto& generator : m_generators )
    {
        auto res = generator->generate( config );
        if( !res.has_error() )
        {
            return outcome::ok<detector_Base::ptr_t>( res.value() );
        }

    }
    return outcome::fail( error::Error_Code::NOT_FOUND,
                          "No driver found for config instance." );
}

} // End of tmns::feature namespace