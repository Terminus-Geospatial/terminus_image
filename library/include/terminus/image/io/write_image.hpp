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
 * @file    write_image.hpp
 * @author  Marvin Smith
 * @date    7/27/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/utility/Subtask_Progress_Callback.hpp>
#include <terminus/math/Point_Utilities.hpp>

// Terminus Image Libraries
#include "../operations/select_plane.hpp"
#include "../types/Image_Memory.hpp"

// C++ Libraries
#include <sstream>

// Boost Libraries
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

namespace tmns::image::io {

/**
 * Write an image to disk for a ROI
 *
 * @param dst
 * @param src
 * @param bbox
*/
template <class PixelT>
Result<void> write_image( Write_Image_Resource_Base&  dst,
                          const Image_Memory<PixelT>& src,
                          const math::Rect2i&         bbox )
{
    dst.write( src.buffer(),
               bbox );
    return outcome::ok();
}

/**
 * Write the entire image to disk.
*/
template <class PixelT>
Result<void> write_image( Image_Resource_Base::ptr_t   dst,
                          const Image_Memory<PixelT>&  src )
{
    return write_image( dst,
                        src,
                        math::Rect2i( 0, 0, src.cols(), src.rows() ) );
}

/**
 * Write an image to disk
*/
template <class ImageT>
Result<void> write_image( Image_Resource_Base::ptr_t dst,
                          const Image_Base<ImageT>&  src,
                          const math::Rect2i&        bbox )
{
    Image_Memory<typename ImageT::pixel_type> intermediate = src;
    return write_image( dst, intermediate, bbox );
}

  /**
   * Write a std::vector of image views.  Supply a filename with an
   * asterisk ('*') and, each image in the vector will be saved as
   * a seperate file on disk with the asterisk will be replaced with
   * the image number.
   */
  template <class ElemT>
  Result<void> write_image( const std::filesystem::path&  filename,
                            const std::vector<ElemT>&     out_image_vector )
  {

    // If there's an asterisk, save one file per plane
    if( !boost::find_last(filename,"*") )
    {
        std::stringstream sout;
        sout << "filename must contain * when writing a vector of images";
        tmns::log::error( sout.str() );
        return outcome::fail( core::error::ErrorCode::INVALID_CONFIGURATION,
                              sout.str() );
    }

    for (unsigned i=0; i<out_image_vector.size(); i++)
    {
        std::string name = filename;
        boost::replace_last( name, "*",  str( boost::format("%1%") % i ) );
        auto write_result = write_image( name, out_image_vector[i] );
        if( write_result.has_error() )
        {
            tmns::log::error( write_result.error().message() );
            return outcome::fail( write_result.error() );
        }
    }

    return outcome::ok();
}

/**
 * Write an image to disk
*/
template <class ImageT>
Result<void> write_image( Image_Resource_Base::ptr_t         resource,
                          const Image_Base<ImageT>&          image,
                          core::utility::Progress_Callback&  progress_callback = core::utility::Progress_Callback::dummy_instance() )
{
    // Check empty resource
    if( image.impl().cols() == 0 ||
        image.impl().rows() == 0 ||
        image.impl().planes() == 0 )
    {
        std::stringstream sout;
        sout << "Cannot write an empty image to a resource.";
        tmns::log::error( sout.str() );
        return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                              sout.str() );
    }

    // Set the progress meter to zero.
    progress_callback.report_progress(0);
    if( progress_callback.abort_requested() )
    {
        return outcome::fail( core::error::ErrorCode::ABORTED,
                              "Aborted by ProgressCallback" );
    }

    const int rows = image.impl().rows();
    const int cols = image.impl().cols();

    // Write the image to disk in blocks.  We may need to revisit
    // the order in which these blocks are rasterized, but for now
    // it rasterizes blocks from left to right, then top to bottom.
    math::Size2i block_size( { cols, rows } );
    if( resource->has_block_write() )
    {
        block_size = resource->block_write_size();
    }

    size_t total_num_blocks = ( ( rows - 1 ) / block_size.height() + 1 )
                            * ( ( cols - 1 ) / block_size.width()  + 1 );

    tmns::log::debug( "writing ", total_num_blocks, " blocks." );

    // Early out for easy case
    if( total_num_blocks == 1 )
    {
        Image_Memory<typename ImageT::pixel_type> image_block = image.impl();
        resource->write( image_block.buffer(),
                         math::Rect2i( 0,
                                       0,
                                       image_block.cols(),
                                       image_block.rows() ) );
    }
    else
    {
        for( int j = 0; j < rows; j+= block_size.height() ) {
        for( int i = 0; i < cols; i+= block_size.width()  ) {

            tmns::log::trace( "writing block at [", i, ", ", j, "]/[", rows, ", ",
                              cols, "] blocksize = ", block_size.width(), " x ",
                              block_size.height() );

            // Rasterize and save this image block
            math::Rect2i current_bbox( math::Point2_<int>( { i, j } ),
                                       math::Point2_<int>( { std::min<int>( i + block_size.width(),
                                                                            cols ),
                                                             std::min<int>( j + block_size.height(),
                                                                            rows ) } ) );

            float processed_row_blocks = float( j / block_size.height() * ( ( cols - 1 ) / block_size.width() + 1 ) );
            float processed_col_blocks = float( i / block_size.width() );
            progress_callback.report_progress( ( processed_row_blocks + processed_col_blocks ) / static_cast<float>( total_num_blocks ) );

            // Rasterize this image block
            Image_Memory<typename ImageT::pixel_type> image_block( crop_image( image.impl(),
                                                                               current_bbox ) );

            Image_Buffer buf = image_block.buffer();
            resource->write( buf, current_bbox );
        }}
    }
    progress_callback.report_finished();

    return outcome::ok();
}

/**
 * @brief Perform a block write uding a disk resource
*/
template <class ImageT>
Result<void> write_image_block( Image_Resource_Base::ptr_t         resource,
                                const Image_Base<ImageT>&          image,
                                core::utility::Progress_Callback&  progress_callback = core::utility::Progress_Callback::dummy_instance() )
{
    if( image.impl().cols() == 0 ||
        image.impl().rows() == 0 ||
        image.impl().planes() == 0 )
    {
        std::stringstream sout;
        sout << "Cannot write an empty image to a resource.";
        tmns::log::error( sout.str() );
        return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                              sout.str() );
    }


    // Set the progress meter to zero.
    progress_callback.report_progress(0);
    if( progress_callback.abort_requested() )
    {
        return outcome::fail( core::error::ErrorCode::ABORTED,
                              "Aborted by ProgressCallback" );
    }

    const int rows = image.impl().rows();
    const int cols = image.impl().cols();

    // Write the image to disk in blocks.  We may need to revisit
    // the order in which these blocks are rasterized, but for now
    // it rasterizes blocks from left to right, then top to bottom.
    math::Size2i block_size( { cols, rows } );
    if( resource->has_block_write() )
    {
        block_size = resource->block_write_size();
    }

    size_t total_num_blocks = ( ( rows - 1 ) / block_size.height() + 1 ) * ( (cols - 1 ) / block_size.width() + 1 );
    tmns::log::debug( "writing ", total_num_blocks, " blocks." );

    // Early out for easy case
    if (total_num_blocks == 1)
    {
        // Use the memory image to just force rasterize everything
        Image_Memory<typename ImageT::pixel_type> image_block = image.impl();
        resource->write( image_block.buffer(),
                         math::Rect2i( 0, 0,
                                       image_block.cols(),
                                       image_block.rows() ) );
    }
    else
    {
        for ( int j = 0; j < rows; j+= block_size.height() ) {
        for ( int i = 0; i < cols; i+= block_size.width() ) {
            std::stringstream sout;
            sout << "writing block at [" << i << " " << j << "]/[" << rows << " " << cols << "] blocksize = "
                 << block_size.width() << " x " <<  block_size.width() << "\n";
            tmns::log::debug( sout.str() );

            // Rasterize and save this image block
            math::Rect2i current_bbox( math::ToPoint2<int>( i, j ),
                                       math::ToPoint2<int>( std::min( i + block_size.width(), cols ),
                                                            std::min( j + block_size.height(), rows ) ) );

            float processed_row_blocks = float( j / block_size.height() * ( ( cols - 1 ) / block_size.width() + 1 ) );
            float processed_col_blocks = float( i / block_size.width() );

            progress_callback.report_progress( ( processed_row_blocks + processed_col_blocks ) / static_cast<float>(total_num_blocks));

            // Rasterize this image block
            Image_Memory<typename ImageT::pixel_type> image_block( crop(image.impl(), current_bbox) );
            Image_Buffer buf = image_block.buffer();
            resource->write( buf, current_bbox );

        }}
    }
    progress_callback.report_finished();
}

/**
 * Write any image type to disk.  If you supply a filename with an asterisk ('*'), each plane
 * of the image will be saved as a seperate file with the asterisk replaced with the plane number.
 */
template <class ImageT>
Result<void> write_image( const std::filesystem::path&             pathname,
                          const Image_Base<ImageT>&                out_image,
                          const std::map<std::string,std::string>& write_options = std::map<std::string,std::string>(),
                          const Disk_Driver_Manager::ptr_t         driver_manager = Disk_Driver_Manager::create_write_defaults(),
                          core::utility::Progress_Callback&        progress_callback = core::utility::Progress_Callback::dummy_instance() )
{
    tmns::log::trace( ADD_CURRENT_LOC(), "Start of Method" );
    Image_Format out_image_format = out_image.format();

    unsigned files = 1;
    // If there's an asterisk, save one file per plane
    if( boost::find_last( pathname.native(), "*" ) )
    {
        files = out_image_format.planes();
        out_image_format.set_planes( 1 );
    }

    for( unsigned p=0; p<files; ++p )
    {
        std::string name = pathname.native();
        if( files > 1 )
        {
            boost::replace_last( name, "*",  str( boost::format("%1%") % p ) );
        }

        {
            std::stringstream sout;
            sout << "Saving image: " << name << ", pixel-type: " << enum_to_string( out_image.channel_type() )
                 << ", channel-type: " << enum_to_string( out_image.format().pixel_type() );
            tmns::log::info( sout.str() );
        }

        // Create an image resource for the data
        math::Size2i block_size( { -1, -1 } ); // Basically ignore
        tmns::log::trace( ADD_CURRENT_LOC(), "Picking Write Driver" );
        auto driver_res = driver_manager->pick_write_driver( pathname,
                                                             out_image_format,
                                                             write_options,
                                                             block_size );
        if( driver_res.has_error() )
        {
            return outcome::fail( driver_res.assume_error() );
        }
        auto resource = driver_res.assume_value();


        if ( files > 1 )
        {
            auto selected_channel = ops::select_plane( out_image.impl(), p );

            auto res = write_image( resource,
                                    selected_channel,
                                    progress_callback );

            progress_callback.report_finished();
            if( res.has_error() )
            {
                tmns::log::error( res.error().message() );
                return outcome::fail( res.error() );
            }
        }
        else
        {
            core::utility::Subtask_Progress_Callback sub_progress_callback( progress_callback,
                                                                            float(p) / float(files),
                                                                            float(p+1) / float(files) );
            auto res = write_image( resource,
                                    ops::select_plane( out_image.impl(), p ),
                                    sub_progress_callback );

            if( res.has_error() )
            {
                tmns::log::error( res.error().message() );
                return outcome::fail( res.error() );
            }
        }
    }
    return outcome::ok();
}

} // End of tmns::image::io namespace