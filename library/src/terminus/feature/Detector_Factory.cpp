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
 * @file    Detector_Factory.cpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#include "Detector_Factory.hpp"

namespace tmns::feature {

/********************************************************/
/*              Create a new feature detector           */
/********************************************************/
Result<Detector_Base::ptr_t> Detector_Factory::create_detector( Detector_Config_Base::ptr_t config ) const
{
    // check null
    if( !config )
    {
        return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                              "Configuration must be initialized.  Cannot be null." );
    }

    // Iterate over each generator
    for( const auto& generator : m_generators )
    {
        auto res = generator->generate( config );
        if( !res.has_error() )
        {
            return outcome::ok<Detector_Base::ptr_t>( res.value() );
        }

    }
    return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                          "No driver found for config instance." );
}


/********************************************************/
/*              Create a new feature detector           */
/********************************************************/
Result<Detector_Base::ptr_t> Detector_Factory::create_extractor( Detector_Config_Base::ptr_t config ) const
{
    // check null
    if( !config )
    {
        return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                              "Configuration must be initialized.  Cannot be null." );
    }

    // Iterate over each generator
    for( const auto& generator : m_generators )
    {
        auto res = generator->generate( config );
        if( !res.has_error() )
        {
            return outcome::ok<Detector_Base::ptr_t>( res.value() );
        }

    }
    return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                          "No driver found for config instance." );
}

} // End of tmns::feature namespace