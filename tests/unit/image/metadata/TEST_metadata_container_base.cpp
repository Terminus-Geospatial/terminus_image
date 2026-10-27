/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2026 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    TEST_metadata_container_base.cpp
 * @author  Marvin Smith
 * @date    9/27/2026
 */

// GoogleTest Libraries
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/image/metadata/metadata_container_base.hpp>

/****************************************/
/*          Read Flat Metadata          */
/****************************************/
TEST( Metadata_Container_Base, reads_flat_metadata_key )
{
    tmns::image::meta::Metadata_Container_Base metadata;
    ASSERT_FALSE( metadata.insert( "projection", "EPSG:4326" ).has_error() );

    auto result = metadata.get<std::string>( "projection" );
    ASSERT_FALSE( result.has_error() );
    EXPECT_EQ( result.value(), "EPSG:4326" );
}

/****************************************/
/*          Read Nested Metadata        */
/****************************************/
TEST( Metadata_Container_Base, reads_nested_metadata_path )
{
    tmns::image::meta::Metadata_Container_Base metadata;
    ASSERT_FALSE( metadata.insert( "gdal.footprint.min_lon", -115.0 ).has_error() );
    ASSERT_FALSE( metadata.insert( "gdal.footprint.min_lat", 35.0 ).has_error() );

    auto longitude = metadata.get<double>( "gdal.footprint.min_lon" );
    auto latitude = metadata.get<double>( "gdal.footprint.min_lat" );
    ASSERT_FALSE( longitude.has_error() );
    ASSERT_FALSE( latitude.has_error() );
    EXPECT_DOUBLE_EQ( longitude.value(), -115.0 );
    EXPECT_DOUBLE_EQ( latitude.value(), 35.0 );
}

/****************************************/
/*          Reject Missing Metadata     */
/****************************************/
TEST( Metadata_Container_Base, rejects_missing_metadata_path )
{
    tmns::image::meta::Metadata_Container_Base metadata;
    ASSERT_FALSE( metadata.insert( "gdal.footprint.min_lon", -115.0 ).has_error() );

    EXPECT_TRUE( metadata.get<double>( "gdal.footprint.max_lon" ).has_error() );
}
