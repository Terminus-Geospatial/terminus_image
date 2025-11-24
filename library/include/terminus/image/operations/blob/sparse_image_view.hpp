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
 * @file    Sparse_Image_View.hpp
 * @author  Marvin Smith
 * @date    8/13/2023
*/
#pragma once

// Terminus Image Libraries
#include "../../pixel/Pixel_Accessor_Loose.hpp"
#include "../../types/Image_Base.hpp"
#include "../rasterize.hpp"
#include "Base_Blob.hpp"

// C++ Libraries
#include <deque>

namespace tmns::image::ops::blob {

/**
 * Processes pixel data from "blobs"
*/
template <typename ImageT,
          typename BlobT>
class Sparse_Image_View : public Image_Base<Sparse_Image_View<ImageT,BlobT>>
{
    public:

        /// Pixel-Type
        typedef typename ImageT::pixel_type pixel_type;

        /// Result of pixel operations
        typedef pixel_type result_type;

        /// Pixel Accessor
        typedef Pixel_Accessor_Loose<Sparse_Image_View<ImageT,BlobT>> pixel_accessor;

        /**
         * Constructor
         */
        Sparse_Image_View( const Image_Base<ImageT>&          under_image,
                           std::deque<std::shared_ptr<BlobT>> blobs )
            : m_under_image( under_image.impl() ),
              m_blobs( blobs )
        {
        }

        /**
         * Get the list of blobs
        */
        std::deque<std::shared_ptr<BlobT>> blobs() const
        {
            return m_blobs;
        }

        /**
         * Image Columns
         */
        size_t cols() const
        {
            return m_under_image.cols();
        }

        /**
         * Image Rows
         */
        size_t rows() const
        {
            return m_under_image.rows();
        }

        /**
         * Image Planes
         */
        size_t planes() const
        {
            return 1;
        }

        /**
         * Get the origin of the image
         */
        pixel_accessor origin() const
        {
            return pixel_accessor( *this, 0, 0 );
        }

        /**
         * Function Operator
         */
        result_type operator()( size_t col, 
                                size_t row,
                                size_t plane ) const
        {
            // Iterate over each blob, checking if it contains the pixel
            for( const auto& blob : m_blobs )
            {
                if( blob->is_inside( col, row, plane ) )
                {
                    return blob->get_pixel( col,
                                            row,
                                            plane ).value();
                }
            }
            return m_under_image( col, row, plane );
        }

        /**
         * Pre-Rasterize Method
         */
        typedef Sparse_Image_View<ImageT,BlobT> prerasterize_type;
        prerasterize_type prerasterize( const tmns::math::Rect2i& bbox ) const
        {
            return (*this);
        }

        /**
         * Rasterize Method
         */
        template <typename DestImageT>
        void rasterize( const DestImageT&         dest,
                        const tmns::math::Rect2i& bbox ) const
        {
            ops::rasterize( prerasterize( bbox ),
                            dest,
                            bbox );
        }

    private:

        /// Underlying Image
        ImageT m_under_image;

        /// Pixel Data
        std::deque<std::shared_ptr<BlobT>> m_blobs;
}; // End of Sparse_Image_View Class

} // End of tmns::image::ops::blob namespace