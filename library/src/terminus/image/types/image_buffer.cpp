/**
 * @file    Image_Buffer.cpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#include "Image_Buffer.hpp"

// Terminus Image Libraries
#include "../pixel/Channel_Type_Enum.hpp"
#include "../pixel/Pixel_Format_Enum.hpp"

// Terminus Libraries
#include <terminus/log/utility.hpp>

// C++ Libraries
#include <iostream>
#include <sstream>

namespace tmns::image {

/********************************/
/*          Constructor         */
/********************************/
Image_Buffer::Image_Buffer( Image_Format format,
                            void*        data )
  : m_data( data ),
   m_format( std::move( format ) ),
   m_cstride( channel_size_bytes( format.channel_type() ).assume_value() *
              num_channels( format.pixel_type() ).assume_value() ),
   m_rstride( m_cstride * format.cols() ),
   m_pstride( m_rstride * format.rows() )
{}

/********************************/
/*          Constructor         */
/********************************/
Image_Buffer::Image_Buffer( void*                data,
                            const Image_Format&  format,
                            size_t               cstride,
                            size_t               rstride,
                            size_t               pstride )
  : m_data( data ),
    m_format( std::move( format ) ),
    m_cstride( cstride ),
    m_rstride( rstride ),
    m_pstride( pstride )
{}

/********************************/
/*          Data Pointer        */
/********************************/
void* Image_Buffer::data() const
{
    return m_data;
}

/************************************/
/*          Get Buffer Cols         */
/************************************/
size_t Image_Buffer::cols() const
{
    return m_format.cols();
}

/************************************/
/*          Get Buffer Rows         */
/************************************/
size_t Image_Buffer::rows() const
{
    return m_format.rows();
}

/********************************/
/*      Get Buffer Planes       */
/********************************/
size_t Image_Buffer::planes() const
{
    return m_format.planes();
}

/****************************************/
/*          Get the Pixel Type          */
/****************************************/
Pixel_Format_Enum Image_Buffer::pixel_type() const
{
    return m_format.pixel_type();
}

/****************************************/
/*          Get the Channel Type        */
/****************************************/
Channel_Type_Enum Image_Buffer::channel_type() const
{
    return m_format.channel_type();
}

/****************************/
/*      Image Format        */
/****************************/
Image_Format Image_Buffer::format() const
{
    return m_format;
}

/****************************/
/*      Image Format        */
/****************************/
Image_Format& Image_Buffer::format()
{
    return m_format;
}

/************************/
/*      Column Stride   */
/************************/
ssize_t Image_Buffer::cstride() const
{
    return m_cstride;
}

/************************/
/*        Row Stride    */
/************************/
ssize_t Image_Buffer::rstride() const
{
    return m_rstride;
}

/************************/
/*      Plane Stride    */
/************************/
ssize_t Image_Buffer::pstride() const
{
    return m_pstride;
}

/************************/
/*      Plane Stride    */
/************************/
void Image_Buffer::set_pstride( ssize_t value )
{
    m_pstride = value;
}

/************************************/
/*      Access Pointer at Pixel     */
/************************************/
void* Image_Buffer::operator()( int col, int row, int plane ) const
{
    return ((uint8_t*)m_data) + ( col * cstride() + row * rstride() + plane * pstride() );
}

/********************************************/
/*      Print to Log-Friendly String        */
/********************************************/
std::string Image_Buffer::to_string( size_t offset ) const
{
    std::string gap( offset, ' ' );
    std::stringstream sout;
    sout << gap << "Image_Buffer: " << std::endl;
    sout << format().to_string( offset + 4 );
    sout << gap << " - CStride: " << cstride() << std::endl;
    sout << gap << " - RStride: " << rstride() << std::endl;
    sout << gap << " - PStride: " << pstride() << std::endl;
    return sout.str();
}

} // end tmns::image