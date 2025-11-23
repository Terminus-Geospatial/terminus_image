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
 * @file    GDAL_Utilities.cpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#include "GDAL_Utilities.hpp"

// External Terminus Libraries
#include <terminus/log/Logger.hpp>

// Terminus Libraries
#include "GDAL_Codes.hpp"

// Boost Libraries
#include <boost/algorithm/string/replace.hpp>


namespace tmns::image::io::gdal {

static tmns::log::Logger g_gdal_logger("tmns::imageio::drivers::gdal");
/****************************************/
/*          Get the GDAL Logger         */
/****************************************/
tmns::log::Logger& get_master_gdal_logger()
{
    return g_gdal_logger;
}

/****************************************/
/*          GDAL Error Handler          */
/****************************************/
static void CPL_STDCALL GDAL_Error_Handler( CPLErr      eErrClass,
                                            int         nError,
                                            const char* pszErrorMsg )
{
    boost::log::trivial::severity_level lvl;
    auto& logger = get_master_gdal_logger();

    switch(eErrClass)
    {
        case CE_Debug:
        case CE_Warning:
            lvl = boost::log::trivial::severity_level::warning;
            break;
        default:
            lvl = boost::log::trivial::severity_level::error;
            break;
    }

    std::string message;
    if( pszErrorMsg )
    {
        message = pszErrorMsg;
    }
    boost::replace_all(message, "\n", " ");

    if ( eErrClass == CE_Fatal )
    {
        logger.fatal( "GDAL: ", message, " (code = ", nError, ")" );
        outcome::panic( core::error::ErrorCode::FILE_IO_ERROR,
                        "GDAL: ", message, " (code = ", nError, ")" );
    }
}

/************************************/
/*          Initialize GDAL         */
/************************************/
Result<void> Initialize_GDAL()
{
    CPLPushErrorHandler( GDAL_Error_Handler );
    // If we run out of handles, GDAL errors out. If you have more than 400
    // open, you probably have a bug.
    CPLSetConfigOption("GDAL_MAX_DATASET_POOL_SIZE", "400");
    GDALAllRegister();

    return outcome::ok();
}

/********************************************/
/*          Get the master GDAL mutex       */
/********************************************/
static std::mutex g_gdal_mtx;
std::once_flag init_flag;
std::mutex& get_master_gdal_mutex()
{
    std::call_once( init_flag, [](){
        if( Initialize_GDAL().has_error() )
        {
            get_master_gdal_logger().error( "Failed to initialize GDAL" );
        }
    });

    return g_gdal_mtx;
}

/****************************************************************/
/*      Method for destrying GDAL even if dataset is null       */
/****************************************************************/
void GDAL_Deleter_Null_Okay( GDALDatasetH dataset )
{
    if( dataset )
    {
        ::GDALClose( dataset );
    }
}

/**************************************************************/
/*       Compare the set of color codes against the LUT       */
/**************************************************************/
Result<Pixel_Format_Enum> gdal_driver_to_pixel_type( const std::vector<std::tuple<std::vector<int>,Pixel_Format_Enum>>& reference_lut,
                                                     const std::vector<int>&                                            channel_codes )
{
    size_t counter = 0;
    for( const auto& ref_tup : reference_lut )
    {
        if( std::get<0>( ref_tup ) == channel_codes )
        {
            return outcome::ok<Pixel_Format_Enum>( std::get<1>( ref_tup ) );
        }
    }
    return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                          "No matching color code found for color set: ",
                          ToLogString( channel_codes ) );
}

/****************************************************/
/*      Convert Pixel format to Channel-Type        */
/****************************************************/
Result<Channel_Type_Enum> gdal_pixel_format_to_channel_type( GDALDataType gdal_type )
{
    switch (gdal_type)
    {
        case GDT_Byte:    return Channel_Type_Enum::UINT8;
        case GDT_Int16:   return Channel_Type_Enum::INT16;
        case GDT_UInt16:  return Channel_Type_Enum::UINT16;
        case GDT_Int32:   return Channel_Type_Enum::INT32;
        case GDT_UInt32:  return Channel_Type_Enum::UINT32;
        case GDT_Float32: return Channel_Type_Enum::FLOAT32;
        case GDT_Float64: return Channel_Type_Enum::FLOAT64;
        default:
            return outcome::fail( core::error::ErrorCode::INVALID_CHANNEL_TYPE,
                                  "Unsupported channel-type ( ",
                                  gdal_type, " )" );
    }
}

/****************************************************/
/*      Convert Channel-Type to Pixel format        */
/****************************************************/
Result<GDALDataType> channel_type_to_gdal_pixel_format( Channel_Type_Enum channel_type )
{
    // @todo:  Add remaining data types to this method
    switch( channel_type )
    {
        // byte
        case Channel_Type_Enum::UINT8:
            return outcome::ok<GDALDataType>( GDT_Byte );

        // uint16
        case Channel_Type_Enum::UINT12:
        case Channel_Type_Enum::UINT14:
        case Channel_Type_Enum::UINT16:
            return outcome::ok<GDALDataType>( GDT_UInt16 );

        // int16
        case Channel_Type_Enum::INT16:
            return outcome::ok<GDALDataType>( GDT_Int16 );

        case Channel_Type_Enum::UINT32:
            return outcome::ok<GDALDataType>( GDT_UInt32 );

        case Channel_Type_Enum::INT32:
            return outcome::ok<GDALDataType>( GDT_Int32 );

        case Channel_Type_Enum::FLOAT32:
        case Channel_Type_Enum::FLOAT32Free:
            return outcome::ok<GDALDataType>( GDT_Float32 );

        case Channel_Type_Enum::FLOAT64:
        case Channel_Type_Enum::FLOAT64Free:
            return outcome::ok<GDALDataType>( GDT_Float64 );

        default:
            return outcome::fail( core::error::ErrorCode::INVALID_CHANNEL_TYPE,
                                  "Cannot convert tmns::pixel::Channel_Type_Enum::",
                                  enum_to_string( channel_type ),
                                  " to GDALDataType" );
    }
}

/********************************/
/*          Get driver          */
/********************************/
std::pair<GDALDriver*, bool> gdal_get_driver_locked( const std::filesystem::path& filename,
                                                     bool                         need_create )
{
    bool unsupported_driver = false;
    GDALDriver* driver = nullptr;

    // Open the appropriate GDAL I/O driver, depending on the fileFormat
    // argument specified by the user.
    std::list<std::string> gdal_format_strings = gdal_file_format_from_filename::format( filename );

    for( const auto& gdal_string : gdal_format_strings )
    {
        {
            std::stringstream sout;
            sout <<"Trying to retrieve GDAL Driver with the following type: " << gdal_string;
            get_master_gdal_logger().trace( sout.str() );
        }

        driver = GetGDALDriverManager()->GetDriverByName( gdal_string.c_str() );
        if( driver == nullptr )
        {
            continue;
        }

        if (need_create)
        {
            char** metadata = driver->GetMetadata();
            if( !CSLFetchBoolean( metadata, GDAL_DCAP_CREATE, FALSE ) )
            {
                get_master_gdal_logger().warn( "GDAL driver ", gdal_string, " does not support create." );
                driver = NULL;
                unsupported_driver = true;
            }
        }

        if ( driver != nullptr )
        {
            break;
        }
    }
    if (!driver)
    {
        get_master_gdal_logger().debug( "Could not get GDAL driver for filename:", filename );
    }
    return std::make_pair(driver, unsupported_driver);

} // End of gdal_get_driver_locked

} // end of tmns::image::io::gdal namespace