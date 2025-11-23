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
 * @file    GDAL_Disk_Image_Impl.cpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#include "GDAL_Disk_Image_Impl.hpp"

/// Terminus Libraries
#include "../../../pixel/convert.hpp"
#include "../../../pixel/Channel_Type_Enum.hpp"
#include "GDAL_Utilities.hpp"
#include "ISIS_JSON_Parser.hpp"

/// External Terminus Libraries
#include <terminus/log/utility.hpp>
#include <terminus/outcome/Result.hpp>

/// C++ Libraries
#include <mutex>

// GDAL Libraries
#include <gdal.h>


namespace tmns::image::io::gdal {

/********************************/
/*          Constructor         */
/********************************/
GDAL_Disk_Image_Impl::GDAL_Disk_Image_Impl( const std::filesystem::path& pathname,
                                            const ColorCodeLookupT&      color_reference_lut )
  : m_pathname( pathname ),
    m_color_reference_lut( color_reference_lut )
{
    open( m_pathname );
}

/********************************/
/*          Constructor         */
/********************************/
GDAL_Disk_Image_Impl::GDAL_Disk_Image_Impl( const std::filesystem::path&             pathname,
                                            const Image_Format&                      output_format,
                                            const std::map<std::string,std::string>& write_options,
                                            const math::Size2i&                      block_size,
                                            const ColorCodeLookupT&                  color_reference_lut )
  : m_pathname( pathname ),
    m_color_reference_lut( color_reference_lut )
{
    configure_for_writing( output_format,
                           write_options,
                           block_size );
}

/************************************/
/*          Open the Dataset        */
/************************************/
Result<void> GDAL_Disk_Image_Impl::open( const std::filesystem::path& pathname )
{
    // Lock the global GDAL Context
    std::unique_lock<std::mutex> lck( get_master_gdal_mutex() );
    auto logger = get_master_gdal_logger();
    logger.trace( "Opening dataset for file: ", pathname.native() );

    /// Create the GDAL Dataset
    m_read_dataset.reset( (GDALDataset*)GDALOpen( pathname.native().c_str(), GA_ReadOnly ), GDAL_Deleter_Null_Okay );

    // Make sure it opens okay
    if( !m_read_dataset )
    {
        std::stringstream sout;
        sout << "GDAL: Failed to open dataset " << m_pathname.native();
        logger.warn( sout.str() );
        return outcome::fail( core::error::ErrorCode::FILE_IO_ERROR,
                               sout.str() );
    }

    m_metadata->insert( "pathname", m_pathname.native() );
    m_metadata->insert( "image_read_driver", "GDAL" );

    // Given dataset, get some information
    std::shared_ptr<GDALDataset> dataset( get_dataset_ptr().value() );

    m_pathname = pathname;
    m_format.set_cols( dataset->GetRasterXSize() );
    m_format.set_rows( dataset->GetRasterYSize() );

    // Log the metadata information
    {
        process_metadata( logger,
                          dataset );
    }

    /**
     * Figure out which pixel format to use from the band composition.  This is oddly difficult,
     * so I'm going to start initially with an lookup table and just add as I go.
     */
    std::vector<int> channel_codes;

    for( int i=1; i<= dataset->GetRasterCount(); ++i )
    {
        channel_codes.push_back( dataset->GetRasterBand(i)->GetColorInterpretation() );
    }

    // Match against expected patterns
    bool match_found = false;

    // Add match logic here
    auto pix_res = gdal_driver_to_pixel_type( m_color_reference_lut,
                                              channel_codes );
    if( pix_res.has_error() )
    {
        std::stringstream sout;
        sout << "Unable to determine pixel-type from color-codes.  Check the lookup table.\n";
        sout << " - Image: " << pathname.native() << std::endl;
        sout << " - Actual Values:" << std::endl;
        for( size_t i=0; i<channel_codes.size(); i++ )
        {
            sout << "   - " << i << " -> " << GDALGetColorInterpretationName( (GDALColorInterp)channel_codes[i] ) << std::endl;
        }
        sout << "Will attempt to determine by simple channel counts.";

        logger.trace( sout.str() );

        // Check next set of rules
        if( channel_codes.size() == 1 )
        {
            m_format.set_pixel_type( Pixel_Format_Enum::GRAY );
            m_format.set_planes( 1 );
        }
        else if( channel_codes.size() == 2 )
        {
            m_format.set_pixel_type( Pixel_Format_Enum::GRAYA );
            m_format.set_planes( 1 );
        }
        else if( channel_codes.size() == 3 )
        {
            m_format.set_pixel_type( Pixel_Format_Enum::RGB );
            m_format.set_planes( 1 );
        }
        else if( channel_codes.size() == 4 )
        {
            m_format.set_pixel_type( Pixel_Format_Enum::RGBA );
            m_format.set_planes( 1 );
        }
        else
        {
            m_format.set_pixel_type( Pixel_Format_Enum::SCALAR );
            m_format.set_planes( channel_codes.size() );
        }

    }
    else
    {
        m_format.set_pixel_type( pix_res.value() );
        m_format.set_planes( 1 );
    }

    /// Get the channel-type
    auto ctype = gdal_pixel_format_to_channel_type( dataset->GetRasterBand(1)->GetRasterDataType() );
    if( ctype.has_error() )
    {
        get_master_gdal_logger().error( "Unable to parse channel-type. ", ctype.error().message() );
        return outcome::fail( ctype.error() );
    }
    m_format.set_channel_type( ctype.value() );

    // Color Palette (if supported)
    if( dataset->GetRasterCount() == 1 &&
        dataset->GetRasterBand(1)->GetColorInterpretation() == GCI_PaletteIndex )
    {
        // Setup the pixel and plane info
        m_format.set_pixel_type( Pixel_Format_Enum::RGBA );
        m_format.set_planes( 1 );

        // Fetch the color table and add to table
        GDALColorTable* color_table = dataset->GetRasterBand(1)->GetColorTable();

        m_color_table.resize( color_table->GetColorEntryCount() );
        GDALColorEntry color;
        for( size_t i=0; i<m_color_table.size(); i++ )
        {
            color_table->GetColorEntryAsRGB( i, &color );
            m_color_table[i] = PixelRGBA_u8( color.c1, color.c2, color.c3, color.c4 );
        }
    }

    // Get the block size
    m_blocksize = default_block_size();

    return outcome::ok();
}

/********************************************/
/*          Read memory from disk           */
/********************************************/
Result<void> GDAL_Disk_Image_Impl::read( const Image_Buffer&  dest,
                                         const math::Rect2i&  bbox,
                                         bool                 rescale ) const
{
    // Perform bounds checks
    if( !format().bbox().is_inside( bbox ) )
    {
        return outcome::fail( core::error::ErrorCode::OUT_OF_BOUNDS,
                              "Bounding box outside the bounds of the image. ",
                              format().bbox().to_string(),
                              ", Requested: " + bbox.to_string() );
    }

    // Create source fetching region
    Image_Format src_fmt = format();
    src_fmt.set_cols( bbox.width() );
    src_fmt.set_rows( bbox.height() );

    std::shared_ptr<uint8_t> src_data(new uint8_t[ src_fmt.raster_size_bytes() ]);
    Image_Buffer src(src_fmt, src_data.get());

    {
        std::unique_lock<std::mutex> lck( get_master_gdal_mutex() );

        auto dataset = get_dataset_ptr().value();

        auto& logger = get_master_gdal_logger();

        auto ch_size = channel_size_bytes( src.format().channel_type() ).value();
        if( m_color_table.empty() )
        {
            auto nchannels = num_channels( format().pixel_type() ).value();
            for( size_t p = 0; p < format().planes();   ++p ) {
            for( size_t c = 0; c < nchannels; ++c )
            {
                // Only one of channels() or planes() will be nonzero.
                auto* band = dataset->GetRasterBand( c + p + 1 );

                auto gdal_pix_fmt = channel_type_to_gdal_pixel_format( format().channel_type() ).value();
                CPLErr result = band->RasterIO( GF_Read,
                                                bbox.min().x(),
                                                bbox.min().y(),
                                                bbox.width(),
                                                bbox.height(),
                                                ( uint8_t* ) src( 0, 0, p ) + ch_size * c,
                                                src.format().cols(),
                                                src.format().rows(),
                                                gdal_pix_fmt,
                                                src.cstride(),
                                                src.rstride() );
                if( result != CE_None )
                {
                    logger.warn( "RasterIO problem: ",
                                 CPLGetLastErrorMsg() );
                }
            }}
        }

        // Convert the color table
        else
        {
            GDALRasterBand* band = dataset->GetRasterBand(1);
            uint8_t* index_data = new uint8_t[ bbox.width() * bbox.height() ];
            CPLErr result = band->RasterIO( GF_Read, bbox.min().x(), bbox.min().y(), bbox.width(), bbox.height(),
                                           index_data, bbox.width(), bbox.height(), GDT_Byte, 1, bbox.width() );
            if (result != CE_None)
            {
                logger.warn( "RasterIO problem: ",
                                 CPLGetLastErrorMsg() );
            }


            PixelRGBA_u8* rgba_data = (PixelRGBA_u8*) src.data();
            for( int i=0; i<bbox.width()*bbox.height(); ++i )
            {
                rgba_data[i] = m_color_table[index_data[i]];
            }

            delete [] index_data;
        }
    }

    return convert( dest, src, rescale );
}

/****************************************************/
/*          Write the image buffer to disk          */
/****************************************************/
Result<void> GDAL_Disk_Image_Impl::write( const Image_Buffer& source_buffer,
                                          const math::Rect2i& bbox,
                                          bool                rescale )
{
    auto dest_format = format();
    dest_format.set_cols( bbox.width() );
    dest_format.set_rows( bbox.height() );

    // Create output buffer
    std::vector<uint8_t> dest_data( dest_format.raster_size_bytes() );
    Image_Buffer dest_buffer( dest_format, dest_data.data() );

    auto res = convert( dest_buffer,
                        source_buffer,
                        rescale );
    if( res.has_error() )
    {
        tmns::log::error( "Problem inside write operation: ", res.error().message() );
        return outcome::fail( res.error() );
    }

    {
        std::unique_lock<std::mutex> lock( get_master_gdal_mutex() );

        // Get the underlying GDAL datatype
        auto gdal_res = channel_type_to_gdal_pixel_format( format().channel_type() );
        if( gdal_res.has_error() )
        {
            return outcome::fail(gdal_res.error());
        }
        GDALDataType gdal_pix_fmt = gdal_res.value();

        // Make sure we have valid channel count
        auto channels = num_channels( dest_buffer.format().pixel_type() ).value();
        auto ch_size_bytes = channel_size_bytes( dest_buffer.format().channel_type() ).value();

        // Iterate over the pixel bands
        for( size_t p = 0; p < dest_buffer.format().planes(); p++ ){
        for( size_t c = 0; c < channels; c++ ){

            GDALRasterBand *band = get_dataset_ptr().value()->GetRasterBand(c+p+1);

            CPLErr result = band->RasterIO( GF_Write,
                                            bbox.min().x(),
                                            bbox.min().y(),
                                            bbox.width(),
                                            bbox.height(),
                                            (uint8_t*)dest_buffer(0,0,p) + ch_size_bytes * c,
                                            dest_buffer.format().cols(),
                                            dest_buffer.format().rows(),
                                            gdal_pix_fmt,
                                            dest_buffer.cstride(),
                                            dest_buffer.rstride() );
            if (result != CE_None)
            {
                std::stringstream sout;
                sout << "RasterIO trouble: '" << CPLGetLastErrorMsg();
                get_master_gdal_logger().error( sout.str() );
                return outcome::fail( core::error::ErrorCode::GDAL_FAILURE,
                                      sout.str() );
            }
        }} // End of for each row/col
    } // End of locked region

    return outcome::ok();
} // End of method

/************************************************/
/*          Print to log-friendly string        */
/************************************************/
std::string GDAL_Disk_Image_Impl::To_Log_String( size_t offset ) const
{
    std::string gap( offset, ' ' );
    std::stringstream sout;
    sout << gap << "   - pathname: " << m_pathname << std::endl;
    sout << gap << "   - read dataset set : " << std::boolalpha << (m_read_dataset != 0) << std::endl;
    sout << gap << "   - write dataset set: " << std::boolalpha << (m_write_dataset != 0) << std::endl;
    sout << m_format.to_string( offset + 2 );
    sout << gap << "   - Block Size: " << m_blocksize.to_string() << std::endl;
    sout << gap << "   - Color Table Size: " << m_color_table.size() << std::endl;
    return sout.str();
}

/********************************************/
/*          Get the Format Structure        */
/********************************************/
Image_Format GDAL_Disk_Image_Impl::format() const
{
    return m_format;
}

/************************************************************/
/*          Get the pointer to the underlying dataset       */
/************************************************************/
Result<GDAL_Disk_Image_Impl::DatasetPtrT> GDAL_Disk_Image_Impl::get_dataset_ptr() const
{
    if( m_write_dataset )
    {
        return outcome::ok<DatasetPtrT>( m_write_dataset );
    }
    else if( m_read_dataset )
    {
        return outcome::ok<DatasetPtrT>( m_read_dataset );
    }
    else
    {
        return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                              "GDAL:  No dataset opened." );
    }
}

/************************************************/
/*          Get the default block size          */
/************************************************/
math::Size2i GDAL_Disk_Image_Impl::default_block_size() const
{
    auto dataset = get_dataset_ptr().value();

    GDALRasterBand *band = dataset->GetRasterBand(1);
    int xsize, ysize;
    band->GetBlockSize( &xsize, &ysize );

    // GDAL assumes a single-row stripsize even for file formats like PNG for
    // which it does not support true strip access. If it looks like it did
    // that (single-line block) only trust it if it's on the whitelist.
    if( ysize == 1 && !blocksize_whitelist( dataset->GetDriver() ) )
    {
        xsize = format().cols();
        ysize = format().rows();
    }

    return std::move( math::Size2i( { xsize, ysize } ) );
}

/*********************************************/
/*      Check if Nodata Read Supported       */
/*********************************************/
bool GDAL_Disk_Image_Impl::has_nodata_read() const
{
    double value;
    return (!nodata_read_ok().has_error());
}

/***********************************************/
/*          Get the block read size            */
/***********************************************/
math::Size2i  GDAL_Disk_Image_Impl::block_read_size() const
{
    return m_blocksize;
}

/************************************************/
/*          Get the block write size            */
/************************************************/
math::Size2i  GDAL_Disk_Image_Impl::block_write_size() const
{
    return m_blocksize;
}

/************************************************/
/*          Set the block write size            */
/************************************************/
void GDAL_Disk_Image_Impl::set_block_write_size( const math::Size2i& block_size )
{
    m_blocksize = block_size;
    std::unique_lock<std::mutex> lock( get_master_gdal_mutex() );
    initialize_write_resource_locked();
}

/****************************************/
/*          Get Nodata Read Value       */
/****************************************/
double GDAL_Disk_Image_Impl::nodata_read() const
{
    auto val = nodata_read_ok();
    if( val.has_error() )
    {
        throw std::runtime_error( "Error reading dataset.  This dataset does not have nodata set." );
    }
    return val.value();
}

/****************************************/
/*      Set the nodata write value      */
/****************************************/
void GDAL_Disk_Image_Impl::set_nodata_write( double value )
{
    std::unique_lock<std::mutex> lock( get_master_gdal_mutex() );
    auto dataset = get_dataset_ptr();
    if( dataset.value()->GetRasterBand(1)->SetNoDataValue( value ) != CE_None )
    {
        std::stringstream sout;
        sout << "GDAL_Disk_Image_Impl: Unable to set nodata value";
        get_master_gdal_logger().error( sout.str() );
        throw std::runtime_error( sout.str() );
    }
}

/************************************************/
/*          Flush and Write Everything          */
/************************************************/
void GDAL_Disk_Image_Impl::flush()
{
    if( m_write_dataset )
    {
        std::unique_lock<std::mutex> lock( get_master_gdal_mutex() );
        m_write_dataset.reset();
    }
}

/************************************/
/*          Get Metadata            */
/************************************/
meta::Metadata_Container_Base::ptr_t GDAL_Disk_Image_Impl::metadata() const
{
    return m_metadata;
}

/************************************************************************************/
/*          Get the list of supported/trusted drivers that rely on blocksizes       */
/************************************************************************************/
bool GDAL_Disk_Image_Impl::blocksize_whitelist( const GDALDriver* driver )
{
    // These drivers are mostly known to report good data
    static const size_t DRIVER_COUNT = 4;
    static const char drivers[DRIVER_COUNT][7] = {"GTiff", "ISIS3", "JP2ECW", "JP2KAK"};

    for (size_t i = 0; i < DRIVER_COUNT; ++i)
    {
        if (driver == GetGDALDriverManager()->GetDriverByName(drivers[i]))
        {
            return true;
        }
    }
    return false;
}

/****************************************************/
/*          Check if GDAL Supports Driver           */
/****************************************************/
bool GDAL_Disk_Image_Impl::gdal_has_support( const std::string& filename )
{
    std::unique_lock<std::mutex> lock( get_master_gdal_mutex() );
    std::pair<GDALDriver *, bool> ret = gdal_get_driver_locked( filename, false );
    return bool(ret.first);
  }

/*********************************************/
/*          Initialize Output Driver         */
/*********************************************/
void GDAL_Disk_Image_Impl::initialize_write_resource_locked()
{
    if( m_write_dataset )
    {
        m_write_dataset.reset();
    }

    int num_bands = std::max( format().planes(),
                              (size_t)num_channels( format().pixel_type() ).value() );

    // returns Maybe driver, and whether it
    // found a ro driver when a rw one was requested
    std::pair<GDALDriver *, bool> ret = gdal_get_driver_locked( m_pathname, true );

    if( ret.first == NULL )
    {
        if( ret.second )
        {
            std::stringstream sout;
            sout << "Could not write: " << m_pathname << ".  Selected GDAL driver not supported.";
            get_master_gdal_logger().error( sout.str() );
            throw std::runtime_error( sout.str() );
        }
        else
        {
            std::stringstream sout;
            sout << "Error opening selected GDAL file I/O driver.";
            get_master_gdal_logger().error( sout.str() );
            throw std::runtime_error( sout.str() );
        }
    }

    GDALDriver *driver = ret.first;
    char **options = NULL;

    // Note:  These calls to assign "options" merely update the pointer with more info appended to it.

    if( format().pixel_type() == Pixel_Format_Enum::GRAYA ||
        format().pixel_type() == Pixel_Format_Enum::RGBA )
    {
        options = CSLSetNameValue( options, "ALPHA", "YES" );
    }
    if( format().pixel_type() != Pixel_Format_Enum::SCALAR )
    {
        options = CSLSetNameValue( options, "INTERLEAVE", "PIXEL" );
    }
    if( format().pixel_type() == Pixel_Format_Enum::RGB ||
        format().pixel_type() == Pixel_Format_Enum::RGBA ||
        format().pixel_type() == Pixel_Format_Enum::GENERIC_3_CHANNEL ||
        format().pixel_type() == Pixel_Format_Enum::GENERIC_4_CHANNEL)
    {
        options = CSLSetNameValue( options, "PHOTOMETRIC", "RGB" );
    }

    // If the user has specified a block size, we set the option for it here.
    if ( m_blocksize[0] != -1 && m_blocksize[1] != -1 )
    {
        std::ostringstream x_str, y_str;
        x_str << m_blocksize[0];
        y_str << m_blocksize[1];
        options = CSLSetNameValue( options, "TILED", "YES" );
        options = CSLSetNameValue( options, "BLOCKXSIZE", x_str.str().c_str() );
        options = CSLSetNameValue( options, "BLOCKYSIZE", y_str.str().c_str() );
    }

    for( const Options::value_type& i : m_driver_options )
    {
        options = CSLSetNameValue( options, i.first.c_str(), i.second.c_str() );
    }

    GDALDataType gdal_pix_fmt = channel_type_to_gdal_pixel_format( format().channel_type() ).value();

    m_write_dataset.reset( driver->Create( m_pathname.c_str(),
                                           format().cols(),
                                           format().rows(),
                                           num_bands,
                                           gdal_pix_fmt,
                                           options ),
                                           GDAL_Deleter_Null_Okay );
    CSLDestroy( options );

    if ( m_blocksize.width() == -1 ||
         m_blocksize.height() == -1 )
    {
        m_blocksize = default_block_size();
    }
}

/*****************************************************/
/*           Check if nodata read was okay           */
/*****************************************************/
Result<double> GDAL_Disk_Image_Impl::nodata_read_ok() const
{
    std::unique_lock<std::mutex> lck( get_master_gdal_mutex() );

    auto dataset = get_dataset_ptr().value();
    int success;
    auto value = dataset->GetRasterBand(1)->GetNoDataValue( &success );
    if( !success )
    {
        return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                              "nodata unsupported by driver." );
    }

    auto image_fmt = this->format();

    // This is a bugfix. If the image has float values,
    // must cast the nodata value to float before exporting
    // it as double. Sometimes it is a float with extra noise
    // which needs to be cleaned up.
    if( image_fmt.channel_type() == Channel_Type_Enum::FLOAT32 )
    {
        value = std::max( float(value),
                          -std::numeric_limits<float>::max() );
    }

    return outcome::ok<double>( value );
}

/********************************************/
/*          Configure for Writing           */
/********************************************/
void GDAL_Disk_Image_Impl::configure_for_writing( const Image_Format&                      output_format,
                                                  const std::map<std::string,std::string>& write_options,
                                                  const math::Size2i&                      block_size )
{
    // Error Check
    //    ref:  VW_ASSERT(format.planes == 1 || format.pixel_format==VW_PIXEL_SCALAR,
    if( output_format.planes() != 1 && output_format.channels() > 1 )
    {
        std::stringstream sout;
        sout << "The image cannot have both multiple channels and multiple planes.";
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }

    if( ( block_size.width() >= 0      || block_size.height() >= 0 ) &&
        ( block_size.width() % 16 != 0 || block_size.height() % 16 != 0 ) )
    {
        std::stringstream sout;
        sout << "Cannot initialize GDAL_Disk_Image_Impl class.  Block dimensions must";
        sout << " be a multiple of 16.";
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }

    // Store away relevent information into the internal data
    // structure for this DiskImageResource
    m_format    = output_format;
    m_blocksize = block_size;

    m_driver_options = write_options;

    if( m_driver_options["PREDICTOR"].empty() )
    {
        // Unless predictor was explicitly set, use predictor 3 for
        // compression of float/double, and predictor 2 for integers,
        // except whose size is one byte, as for those compression makes
        // things worse.
        if( format().channel_type() == Channel_Type_Enum::FLOAT32 ||
            format().channel_type() == Channel_Type_Enum::FLOAT64 )
        {
            m_driver_options["PREDICTOR"] = "3";
        }
        else if( is_integer_type( format().channel_type() ) )
        {
            m_driver_options["PREDICTOR"] = "2";
        }
        else
        {
            m_driver_options["PREDICTOR"] = "1"; // Must not leave unset
        }
    }
    else
    {
        m_driver_options["PREDICTOR"] = "1"; // Must not leave unset
    }

    std::unique_lock<std::mutex> lck( get_master_gdal_mutex() );
    initialize_write_resource_locked();
}

/****************************************/
/*          Process Metadata            */
/****************************************/
void GDAL_Disk_Image_Impl::process_metadata( log::Logger&                 logger, 
                                             std::shared_ptr<GDALDataset> dataset )
{
    const bool DO_NOT_OVERWRITE { false };
    
    logger.trace( "Metadata Description: ", dataset->GetDescription() );
    m_metadata->insert( "gdal.description", dataset->GetDescription() );

    // Get a list of metadata domains
    auto metadata_domains = dataset->GetMetadataDomainList();
    logger.trace( "Domains: ", CSLCount( metadata_domains ) );
    std::vector<std::string> domain_list;
    if( CSLCount( metadata_domains ) > 0 )
    {
        std::stringstream sout;
        sout << "Domain Items, Count: " << CSLCount( metadata_domains ) << std::endl;
        for( int i = 0; i < CSLCount( metadata_domains ); i++ )
        {
            domain_list.push_back( CSLGetField( metadata_domains, i ) );
            sout << "\t\t[" << domain_list.back() << "]" << std::endl;
        }
        logger.trace( sout.str() );
    }

    char** metadata = dataset->GetMetadata();
    logger.trace( "Count: ", CSLCount(metadata) );

    if( CSLCount(metadata) > 0 )
    {
        std::stringstream sout;
        sout << "Metadata Items, Count: " << CSLCount( metadata ) << std::endl;
        for( int i = 0; i < CSLCount( metadata ); i++ )
        {
            sout << "\t\t" << CSLGetField( metadata, i ) << std::endl;
        }
        logger.trace( sout.str() );
    }

    for( const auto& domain : domain_list )
    {
        char** dmetadata = dataset->GetMetadata( domain.c_str() );
        logger.trace( "Count: ", CSLCount( dmetadata ) );

        if( CSLCount( dmetadata ) > 0 && domain != "json:ISIS3" )
        {
            std::stringstream sout;
            sout << "Domain [" << domain << "] Metadata Items, Count: " << CSLCount( dmetadata ) << std::endl;
            for( int i = 0; i < CSLCount( dmetadata ); i++ )
            {
                sout << "\t\t" << CSLGetField( dmetadata, i ) << std::endl;
            }
            logger.trace( sout.str() );
        }
        else if( domain == "json:ISIS3" )
        {
            logger.debug( "Parsing ISIS3 JSON Node" );
            for( int i = 0; i < CSLCount( dmetadata ); i++ )
            {
                auto result = ISIS_JSON_Parser::parse( CSLGetField( dmetadata, i ) );
                if( result.has_error() )
                {
                    logger.error( "Trouble parsing ISIS JSON data.",
                                  result.error().message() );
                }
                m_metadata->insert( result.value(), DO_NOT_OVERWRITE );
            }
        }
    }

    logger.trace( "Driver: ", dataset->GetDriver()->GetDescription(),
                  ", ", dataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
    m_metadata->insert( "gdal.driver.name_short", dataset->GetDriver()->GetDescription() );
    m_metadata->insert( "file_driver", dataset->GetDriver()->GetDescription() );
    m_metadata->insert( "gdal.driver.name_long",  dataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );

    logger.trace( "Image Size: ", dataset->GetRasterXSize(), " cols, ",
                  dataset->GetRasterYSize(), " rows, ", dataset->GetRasterCount(),
                  " channels" );
}

} // End of tmns::image::io::gdal namespace