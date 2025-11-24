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
 * @file    GDAL_Utilities.hpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#pragma once

// External Terminus Libraries
#include <terminus/log/Logger.hpp>
#include <terminus/outcome/Result.hpp>

// Terminus Libraries
#include <terminus/image/pixel/Channel_Type_Enum.hpp>
#include <terminus/image/pixel/Pixel_Format_Enum.hpp>

// GDAL Libraries
#include <gdal.h>
#include <gdal_priv.h>

// Boost Libraries
#include <boost/algorithm/string.hpp>

// C++ Libraries
#include <list>
#include <mutex>
#include <tuple>
#include <vector>

namespace tmns::image::io::gdal {

/**
 * Initialize GDAL for use
*/
Result<void> Initialize_GDAL();

/**
 * Get a reference to the master GDAL lock
*/
std::mutex& get_master_gdal_mutex();

/**
 * @brief Get a reference to the logger and mutex
*/
log::Logger& get_master_gdal_logger();

/**
 * Method for destrying GDAL even if dataset is null
*/
void GDAL_Deleter_Null_Okay( GDALDatasetH dataset );

/**
 * Method to convert the GDAL Raster Codes into a composite pixeltype.
*/
Result<Pixel_Format_Enum> gdal_driver_to_pixel_type( const std::vector<std::tuple<std::vector<int>,Pixel_Format_Enum>>& reference_lut,
                                                     const std::vector<int>&                                            channel_codes );


/**
 * Method to convert GDAL data-type object into a Terminus Channel-Type enum
*/
Result<Channel_Type_Enum> gdal_pixel_format_to_channel_type( GDALDataType gdal_type );

/**
 * Method to convert Terminus Channel-Types into GDAL Data-Types
*/
Result<GDALDataType> channel_type_to_gdal_pixel_format( Channel_Type_Enum channel_type );

/**
 * Get the GDAL driver for the specified filename.  Will determine if you can read and write,
 * or just read.
*/
std::pair<GDALDriver*, bool> gdal_get_driver_locked( const std::filesystem::path&  filename,
                                                     bool                          need_create = false );

/**
 * Structure with a mapping of GDAL-supported file formats.
 * See:  https://gdal.org/drivers/raster/index.html
*/
struct gdal_file_format_from_filename
{
    /**
     * Get the list of drivers to use for the given filename
     *
     * Boy I wish GDAL had something like this...
    */
    static std::list<std::string> format( const std::filesystem::path& filename )
    {
        std::list<std::string> retval;
        std::string ext = boost::to_lower_copy( filename.extension().native() );

        // Geo-Tiff
        if (ext == ".tif" || ext == ".tiff")
        {
            retval.push_back("GTiff");
        }

        // GMT Compatible NetCDF
        else if (ext == ".grd")
        {
            retval.push_back("GMT");
        }

        // ENVI Labelled Raster
        else if (ext == ".dem")
        {
            retval.push_back("ENVI");
        }

        // ESRI .hdr labelled
        else if (ext == ".bil")
        {
            retval.push_back("EHdr");
        }

        // JPEG JFIF
        else if (ext == ".jpg" || ext == ".jpeg")
        {
            retval.push_back("JPEG");
        }

        // JPEG2000
        else if (ext == ".jp2" || ext == ".j2k" || ext == ".j2c")
        {
            retval.push_back("JP2KAK");               // kakadu
            retval.push_back("JPEG2000");             // jasper
            retval.push_back("JP2ECW");               // ecwj2k
            retval.push_back("JP2OpenJPEG");          // openjpeg
        }

        // PNG
        else if (ext == ".png")
        {
            retval.push_back("PNG");
        }

        // GIF
        else if (ext == ".gif")
        {
            retval.push_back("GIF");
        }

        // ISIS Cube
        else if (ext == ".cub")
        {
            retval.push_back("ISIS3");
            retval.push_back("ISIS2");
        }

        // Planetary data system image
        else if (ext == ".img" || ext == ".pds" || ext == ".lbl")
        {
            retval.push_back("PDS");
        }
        // USGS SDTS DEM
        else if (ext == ".ddf")
        {
            retval.push_back("SDTS");
        }

        // Arc/Info ASCII Grid
        else if (ext == ".asc")
        {
            retval.push_back("AAIGrid");
        }

        // Arc/Info Binary Grid
        else if (ext == ".adf")
        {
            retval.push_back("AIG");
        }

        // New Labelled USGS DOQ
        else if (ext == ".doq")
        {
            retval.push_back("DOQ2");
        }

        // Military Elevation Data
        else if (ext == ".dt0" || ext == ".dt1" || ext == ".dt2")
        {
            retval.push_back("DTED");
        }

        // FITS (needs GDAL w/ libcfitsio)
        else if (ext == ".fits")
        {
            retval.push_back("FITS");
        }

        // National Image Transmission Format
        else if (ext == ".ntf")
        {
            retval.push_back("NITF");
        }

        // NASA SRTM Format
        else if (ext == ".hgt")
        {
            retval.push_back("SRTMHGT");
        }
        else
        {
            std::stringstream sout;
            sout << "GDAL_Utilities: \"" << ext << "\" is an unsupported file extension.";
            get_master_gdal_logger().error( sout.str() );
            throw std::runtime_error( sout.str() );
        }
        return retval;
    }
}; // End of gdal_file_format_from_filename

} // end of tmns::image::io::gdal namespace