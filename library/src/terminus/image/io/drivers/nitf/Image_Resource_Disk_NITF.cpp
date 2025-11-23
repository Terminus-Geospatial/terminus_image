/**
 * @file    Image_Resource_Disk_NITF.cpp
 * @author  Marvin Smith
 * @date    7/19/2024
*/
#include <terminus/image/io/drivers/nitf/Image_Resource_Disk_NITF.hpp>

// Terminus Libraries
#include <terminus/image/pixel/Pixel_Format_Enum.hpp>

namespace tmns::image::io::nitf {

/********************************/
/*          Constructor         */
/********************************/
Image_Resource_Disk_NITF::Image_Resource_Disk_NITF( const std::filesystem::path& pathname )
  : Image_Resource_Disk( pathname )
{
}

/********************************/
/*          Constructor         */
/********************************/
Image_Resource_Disk_NITF::Image_Resource_Disk_NITF( const std::filesystem::path&             pathname,
                                                    const Image_Format&                      output_format,
                                                    const std::map<std::string,std::string>& write_options,
                                                    const math::Size2i&                      block_size )
  : Image_Resource_Disk( pathname )
{
}

/********************************************/
/*          Get the resource name           */
/********************************************/
std::string Image_Resource_Disk_NITF::resource_name() const
{
    return "NITF";
}

/****************************************************/
/*          Create Resource and Open Image          */
/****************************************************/
Result<Image_Resource_Disk_NITF::ParentPtrT>
        Image_Resource_Disk_NITF::create( const std::filesystem::path& pathname )
{
    auto driver = std::make_shared<Image_Resource_Disk_NITF>( pathname );

    

    return outcome::ok<ParentPtrT>( driver );
}

/****************************************************/
/*          Create Resource and Open Image          */
/****************************************************/
Result<Image_Resource_Disk_NITF::ParentPtrT>
        Image_Resource_Disk_NITF::create(  const std::filesystem::path&             pathname,
                                           const Image_Format&                      output_format,
                                           const std::map<std::string,std::string>& write_options,
                                           const math::Size2i&                      block_size  )
{
    auto driver = std::make_shared<Image_Resource_Disk_NITF>( pathname,
                                                              output_format,
                                                              write_options,
                                                              block_size );
    
    

    return outcome::ok<ParentPtrT>( driver );
}

/************************************/
/*          Open the dataset        */
/************************************/
Result<void> Image_Resource_Disk_NITF::open( const std::filesystem::path& pathname )
{
    return m_impl->open( pathname );
}

/****************************************************/
/*          Read the image buffer from disk         */
/****************************************************/
Result<void> Image_Resource_Disk_NITF::read( const Image_Buffer& dest,
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
Result<void> Image_Resource_Disk_NITF::write( const Image_Buffer& source_buffer,
                                              const math::Rect2i& bbox )
{
    return m_impl->write( source_buffer,
                          bbox,
                          m_rescale );
}

/****************************************/
/*          Get the pixel data          */
/****************************************/
Image_Format  Image_Resource_Disk_NITF::format() const
{
    return m_impl->format();
}

/********************************************/
/*      Check if Block Read Supported       */
/********************************************/
bool Image_Resource_Disk_NITF::has_block_read() const
{
}

/*********************************************/
/*      Check if Block Write Supported       */
/*********************************************/
bool Image_Resource_Disk_NITF::has_block_write() const
{
    return false;
}

/*********************************************/
/*      Check if Nodata Read Supported       */
/*********************************************/
bool Image_Resource_Disk_NITF::has_nodata_read() const
{
    return false;
}

/**********************************************/
/*      Check if Nodata Write Supported       */
/**********************************************/
bool Image_Resource_Disk_NITF::has_nodata_write() const
{
    return false;
}

/***********************************************/
/*          Get the block read size            */
/***********************************************/
math::Size2i  Image_Resource_Disk_NITF::block_read_size() const
{
    return m_impl->block_read_size();
}

/************************************************/
/*          Get the block write size            */
/************************************************/
math::Size2i  Image_Resource_Disk_NITF::block_write_size() const
{
    return m_impl->block_write_size();
}

/************************************************/
/*          Set the block write size            */
/************************************************/
void Image_Resource_Disk_NITF::set_block_write_size( const math::Size2i& block_size )
{
    m_impl->set_block_write_size( block_size );
}

/****************************************/
/*          Get Nodata Read Value       */
/****************************************/
double Image_Resource_Disk_NITF::nodata_read() const
{
    return m_impl->nodata_read();
}

/****************************************/
/*      Set the nodata write value      */
/****************************************/
void Image_Resource_Disk_NITF::set_nodata_write( double value )
{
    m_impl->set_nodata_write( value );
}

/****************************/
/*          Flush           */
/****************************/
void Image_Resource_Disk_NITF::flush()
{
    m_impl->flush();
}

/************************************************/
/*          Print to log-friendly string        */
/************************************************/
std::string Image_Resource_Disk_NITF::to_log_string( size_t offset ) const
{
    std::string gap( offset, ' ' );
    std::stringstream sout;
    sout << gap << " - Image_Resource_Disk_NITF" << std::endl;
    sout << gap << "   - Impl Initialized: " << std::boolalpha << (m_impl != 0) << std::endl;
    if( m_impl )
    {
        sout << m_impl->To_Log_String( offset );
    }
    return sout.str();
}

} // end of tmns::image::io::gdal namespace