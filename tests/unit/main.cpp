/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    7/17/2023
 */
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/log/configure.hpp>
#include <terminus/log/utility.hpp>

// Unit-Test Libraries
#include "UNIT_TEST_ONLY/Image_Datastore.hpp"
#include "UNIT_TEST_ONLY/Options.hpp"
#include "UNIT_TEST_ONLY/Test_Environment.hpp"

int main(int argc, char* argv[], char* envp[] )
{
    // Parse gtest flags first.  If we are only listing tests, skip the
    // heavier Terminus runtime initialization that gtest discovery does
    // not need.
    ::testing::InitGoogleTest(&argc, argv);

    if( ::testing::GTEST_FLAG(list_tests) )
    {
        return RUN_ALL_TESTS();
    }

    // Parse Command-Line Options
    auto options = Options::parse_command_line( argc, argv, envp );

    // Setup the Terminus Logger
    std::istringstream log_config { options.get_log_config() };
    if( !tmns::log::configure( log_config ) )
    {
        tmns::log::error( "Unable to configure logging. Using defaults." );
    }
    tmns::log::info( "Logger initialized" );

    // Load image datastore
    auto image_datastore = Image_Datastore::load( options.get_test_image_config_path() );
    if( !image_datastore )
    {
        tmns::log::warn( "Unable to find image datastore at ",
                         options.get_test_image_config_path(),
                         " expect to skip test which require large images." );
    }

    // Create test environment
    Test_Environment::initialize( image_datastore,
                                  options.use_gui() );

    return RUN_ALL_TESTS();
}

