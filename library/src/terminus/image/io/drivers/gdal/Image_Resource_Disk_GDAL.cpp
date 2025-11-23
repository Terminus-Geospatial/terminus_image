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
 * @file    Image_Resource_Disk_GDAL.cpp
 * @author  Marvin Smith
 * @date    7/15/2023
*/
#include "Image_Resource_Disk_GDAL.hpp"

// Terminus Libraries
#include "../../../pixel/Pixel_Format_Enum.hpp"
#include "GDAL_Disk_Image_Impl.hpp"

namespace tmns::image::io::gdal {

/********************************/
/*          Constructor         */
/********************************/
Image_Resource_Disk_GDAL::Image_Resource_Disk_GDAL( const std::filesystem::path& pathname,
                                                    ColorCodeLookupT             color_reference_lut )
  : Image_Resource_Disk( pathname ),
    m_color_reference_lut( color_reference_lut )
{
    m_impl = std::make_shared<GDAL_Disk_Image_Impl>( pathname,
                                                     color_reference_lut );
}

/********************************/
/*          Constructor         */
/********************************/
Image_Resource_Disk_GDAL::Image_Resource_Disk_GDAL( const std::filesystem::path&             pathname,
                                                    const Image_Format&                      output_format,
                                                    const std::map<std::string,std::string>& write_options,
                                                    const math::Size2i&                      block_size,
                                                    const ColorCodeLookupT&                  color_reference_lut )
  : Image_Resource_Disk( pathname ),
    m_color_reference_lut( color_reference_lut )
{
    m_impl = std::make_shared<GDAL_Disk_Image_Impl>( pathname,
                                                     output_format,
                                                     write_options,
                                                     block_size,
                                                     color_reference_lut );
}

/********************************/
/*          Destructor          */
/********************************/
Image_Resource_Disk_GDAL::~Image_Resource_Disk_GDAL()
{
    m_impl.reset();
}

/********************************************/
/*          Get the resource name           */
/********************************************/
std::string Image_Resource_Disk_GDAL::resource_name() const
{
    return "GDAL";
}

/****************************************************/
/*          Create Resource and Open Image          */
/****************************************************/
Result<Image_Resource_Disk_GDAL::ParentPtrT>
        Image_Resource_Disk_GDAL::create( const std::filesystem::path& pathname )
{
    auto driver = std::make_shared<Image_Resource_Disk_GDAL>( pathname );

    // Update Metadata
    driver->metadata()->insert( driver->m_impl->metadata(),
                                        true );

    return outcome::ok<ParentPtrT>( driver );
}

/****************************************************/
/*          Create Resource and Open Image          */
/****************************************************/
Result<Image_Resource_Disk_GDAL::ParentPtrT>
        Image_Resource_Disk_GDAL::create(  const std::filesystem::path&             pathname,
                                           const Image_Format&                      output_format,
                                           const std::map<std::string,std::string>& write_options,
                                           const math::Size2i&                      block_size,
                                           const ColorCodeLookupT&                  color_reference_lut  )
{
    auto driver = std::make_shared<Image_Resource_Disk_GDAL>( pathname,
                                                              output_format,
                                                              write_options,
                                                              block_size,
                                                              color_reference_lut );
    
    // Update Metadata
    driver->metadata()->insert( driver->m_impl->metadata(),
                                        true );

    return outcome::ok<ParentPtrT>( driver );
}

/************************************/
/*          Open the dataset        */
/************************************/
Result<void> Image_Resource_Disk_GDAL::open( const std::filesystem::path& pathname )
{
    return m_impl->open( pathname );
}

/****************************************************/
/*          Read the image buffer from disk         */
/****************************************************/
Result<void> Image_Resource_Disk_GDAL::read( const Image_Buffer& dest,
                                             const math::Rect2i& bbox ) const
{
    auto result = m_impl->read( dest, bbox, m_rescale );

    // Process metadata
    metadata()->insert( m_impl->metadata(),
                        true );

    return result;
}

/****************************************************/
/*          Write the image buffer to disk          */
/****************************************************/
Result<void> Image_Resource_Disk_GDAL::write( const Image_Buffer& source_buffer,
                                              const math::Rect2i& bbox )
{
    return m_impl->write( source_buffer,
                          bbox,
                          m_rescale );
}

/****************************************/
/*          Get the pixel data          */
/****************************************/
Image_Format  Image_Resource_Disk_GDAL::format() const
{
    return m_impl->format();
}

/********************************************/
/*      Check if Block Read Supported       */
/********************************************/
bool Image_Resource_Disk_GDAL::has_block_read() const
{
    return true;
}

/*********************************************/
/*      Check if Block Write Supported       */
/*********************************************/
bool Image_Resource_Disk_GDAL::has_block_write() const
{
    return true;
}

/*********************************************/
/*      Check if Nodata Read Supported       */
/*********************************************/
bool Image_Resource_Disk_GDAL::has_nodata_read() const
{
    return m_impl->has_nodata_read();
}

/**********************************************/
/*      Check if Nodata Write Supported       */
/**********************************************/
bool Image_Resource_Disk_GDAL::has_nodata_write() const
{
    return true;
}

/***********************************************/
/*          Get the block read size            */
/***********************************************/
math::Size2i  Image_Resource_Disk_GDAL::block_read_size() const
{
    return m_impl->block_read_size();
}

/************************************************/
/*          Get the block write size            */
/************************************************/
math::Size2i  Image_Resource_Disk_GDAL::block_write_size() const
{
    return m_impl->block_write_size();
}

/************************************************/
/*          Set the block write size            */
/************************************************/
void Image_Resource_Disk_GDAL::set_block_write_size( const math::Size2i& block_size )
{
    m_impl->set_block_write_size( block_size );
}

/****************************************/
/*          Get Nodata Read Value       */
/****************************************/
double Image_Resource_Disk_GDAL::nodata_read() const
{
    return m_impl->nodata_read();
}

/****************************************/
/*      Set the nodata write value      */
/****************************************/
void Image_Resource_Disk_GDAL::set_nodata_write( double value )
{
    m_impl->set_nodata_write( value );
}

/****************************/
/*          Flush           */
/****************************/
void Image_Resource_Disk_GDAL::flush()
{
    m_impl->flush();
}

/************************************************/
/*          Print to log-friendly string        */
/************************************************/
std::string Image_Resource_Disk_GDAL::to_log_string( size_t offset ) const
{
    std::string gap( offset, ' ' );
    std::stringstream sout;
    sout << gap << " - Image_Resource_Disk_GDAL" << std::endl;
    sout << gap << "   - Impl Initialized: " << std::boolalpha << (m_impl != 0) << std::endl;
    if( m_impl )
    {
        sout << m_impl->To_Log_String( offset );
    }
    return sout.str();
}

/************************************************/
/*          Check if GDAL Supports File         */
/************************************************/
bool Image_Resource_Disk_GDAL::gdal_has_support( const std::string& filename )
{
    return GDAL_Disk_Image_Impl::gdal_has_support( filename );
}

} // end of tmns::image::io::gdal namespace