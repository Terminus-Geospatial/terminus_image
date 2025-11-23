/**
 * @file    Image_Resource_Base.cpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#include "Image_Resource_Base.hpp"

// Terminus Image Libraries
#include "../pixel/Channel_Type_Enum.hpp"

namespace tmns::image {

/****************************************************/
/*          Get the number of image columns         */
/****************************************************/
size_t Read_Image_Resource_Base::cols() const
{
    return format().cols();
}

/*************************************************/
/*          Get the number of image rows         */
/*************************************************/
size_t Read_Image_Resource_Base::rows() const
{
    return format().rows();
}

/****************************************************/
/*          Get the number of image planes          */
/****************************************************/
size_t Read_Image_Resource_Base::planes() const
{
    return format().planes();
}

/********************************************************/
/*          Get the number of image channels            */
/********************************************************/
size_t Read_Image_Resource_Base::channels() const
{
    return num_channels( pixel_type() ).assume_value();
}

/************************************************/
/*          Get an image bounding box           */
/************************************************/
math::Rect2i Read_Image_Resource_Base::full_bbox() const
{
    return math::Rect2i( 0, 0, cols(), rows() );
}

/************************************************/
/*          Get the Pixel Format Type           */
/************************************************/
Pixel_Format_Enum Read_Image_Resource_Base::pixel_type() const
{
    return format().pixel_type();
}

/****************************************/
/*          Get the Channel Type        */
/****************************************/
Channel_Type_Enum Read_Image_Resource_Base::channel_type() const
{
    return format().channel_type();
}

/********************************************/
/*          Get the block read size         */
/********************************************/
math::Size2i Read_Image_Resource_Base::block_read_size() const
{
    return math::Size2i( { (int)cols(),
                           (int)rows() } );
}

/********************************************/
/*          Get the nodata value            */
/********************************************/
double Read_Image_Resource_Base::nodata_read() const
{
    throw std::runtime_error( "Unsupported feature" );
}

/********************************************/
/*          Get the native pointer          */
/********************************************/
std::shared_ptr<const uint8_t[]> Read_Image_Resource_Base::native_ptr() const
{
    std::shared_ptr<const uint8_t[]> data( new uint8_t[ native_size() ] );
    this->read( Image_Buffer( format(),
                              const_cast<uint8_t*>( data.get() ) ),
                math::Rect2i( 0, 0, cols(), rows() ) );
  return data;
}

/****************************************/
/*          Get the native size         */
/****************************************/
size_t Read_Image_Resource_Base::native_size() const
{
    return channel_size_bytes( channel_type() ).value() *
            num_channels( pixel_type() ).value() * cols() * rows() * planes();
}

/*********************************************/
/*          Get the block write size         */
/*********************************************/
math::Size2i Write_Image_Resource_Base::block_write_size() const
{
    throw std::runtime_error( "This resource does not support block writes." );
}

/********************************************/
/*      Set a specific block write size     */
/********************************************/
void Write_Image_Resource_Base::set_block_write_size( const math::Size2i& sz )
{
    throw std::runtime_error( "This resource does not support block writes." );
}

/****************************************/
/*      Set a specific nodata value     */
/****************************************/
void Write_Image_Resource_Base::set_nodata_write( double value )
{
    throw std::runtime_error( "This resource does not support block writes." );
}

} // End of tmns::image namespace