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
 * @file    rasterize.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// Terminus Methods
#include <terminus/log/utility.hpp>
#include <terminus/math/Rectangle.hpp>

// Terminus Image Methods
#include "../types/Image_Traits.hpp"

namespace tmns::image::ops {


/**
 * Master Rasterization Function
 *
 * This is called by views that do not have specially optimized rasterization
 * methods.  The user can also call it explicitly when pixel-by-pixel rasterization
 * is preferred to the default optimized rasterization behavior.  This can be useful
 * in some cases, such as when the views are heavily subsampled.
 */
template <class SrcT, class DestT>
void rasterize( const SrcT&          src,
                const DestT&         dest,
                const math::Rect2i&  bbox )
{
    typedef typename DestT::pixel_type     DestPixelT;
    typedef typename SrcT::pixel_accessor  SrcAccT;
    typedef typename DestT::pixel_accessor DestAccT;

    // Sanity Checks
    if( ((int)dest.cols()) != bbox.width() ||
        ((int)dest.rows()) != bbox.height() ||
        dest.planes()      != src.planes() )
    {
        std::stringstream sout;
        sout << "rasterize: Source and destination must have same dimensions. Source: "
            << src.cols() << " x " << src.rows() << ", Dest: " << dest.cols() << " x "
            << dest.rows();

        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }

    // Get the plane data
    SrcAccT  splane = src.origin().advance(bbox.min().x(),bbox.min().y());
    DestAccT dplane = dest.origin();
    for( int plane=src.planes(); plane; --plane )
    {
        SrcAccT  srow = splane;
        DestAccT drow = dplane;
        for( int row=bbox.height(); row; --row )
        {
            SrcAccT  scol = srow;
            DestAccT dcol = drow;
            for( int col = bbox.width(); col; --col )
            {
#ifdef __llvm__
                // LLVM doesn't like ProceduralPixelAccessor's operator*
                // that returns a non reference. We can work around if we
                // split the command in two lines.
                DestPixelT buffer(*scol);
                *dcol = buffer;
#else
                *dcol = DestPixelT(*scol);
#endif
                scol.next_col();
                dcol.next_col();
            }
            srow.next_row();
            drow.next_row();
        }
        splane.next_plane();
        dplane.next_plane();
    }
}

/**
 * Helper function to rasterize the entire source image
*/
template <class SrcT, class DestT>
void rasterize( const SrcT&   src,
                const DestT&  dest )
{
    rasterize( src, dest, math::Rect2i( 0, 0, src.cols(), src.rows() ) );
}

/**
 * A specialization for resizable destination views.
 *
 * This function resizes the destination view prior to
 * rasterization.
 */
template <class SrcT, class DestT>
typename std::enable_if<Is_Resizable<DestT>::value, void>::type
    rasterize( const SrcT&          src,
               DestT&               dest,
               const math::Rect2i&  bbox )
{
    dest.set_size( bbox.width(),
                   bbox.height(),
                   src.planes() );
    rasterize( src, const_cast<DestT const&>(dest), bbox );
}

/**
 * Convenience function to rasterize entire image, if resizable
*/
template <class SrcT, class DestT>
typename std::enable_if<Is_Resizable<DestT>::value, void>::type
    rasterize( const SrcT& src, DestT& dest )
{
    dest.set_size( src.cols(), src.rows(), src.planes() );
    rasterize( src, const_cast<DestT const&>(dest), math::Rect2i( 0, 0, src.cols(), src.rows() ) );
}

} // End of tmns::image::ops namespace