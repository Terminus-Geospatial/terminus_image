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
#include "../../pixel/pixel_accessor_loose.hpp"
#include "../../types/image_base.hpp"
#include "../rasterize.hpp"
#include "base_blob.hpp"

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

        /// @brief Pixel-Type
        typedef typename ImageT::pixel_type pixel_type;

        /// @brief Result of pixel operations
        typedef pixel_type result_type;

        /// @brief Pixel Accessor
        typedef Pixel_Accessor_Loose<Sparse_Image_View<ImageT,BlobT>> pixel_accessor;

        /**
         * @brief Constructor
         *
         * @param under_image Underlying image
         * @param blobs List of blobs
         */
        Sparse_Image_View( const Image_Base<ImageT>&          under_image,
                           std::deque<std::shared_ptr<BlobT>> blobs )
            : m_under_image( under_image.impl() ),
              m_blobs( blobs )
        {
        }

        /**
         * @brief Get the list of blobs
         *
         * @return std::deque<std::shared_ptr<BlobT>> List of blobs
         */
        std::deque<std::shared_ptr<BlobT>> blobs() const
        {
            return m_blobs;
        }

        /**
         * @brief Image Columns
         *
         * @return size_t Number of columns
         */
        size_t cols() const
        {
            return m_under_image.cols();
        }

        /**
         * @brief Image Rows
         *
         * @return size_t Number of rows
         */
        size_t rows() const
        {
            return m_under_image.rows();
        }

        /**
         * @brief Image Planes
         *
         * @return size_t Number of planes (always 1 for sparse images)
         */
        size_t planes() const
        {
            return 1;
        }

        /**
         * @brief Get the origin of the image
         *
         * @return pixel_accessor Origin pixel accessor
         */
        pixel_accessor origin() const
        {
            return pixel_accessor( *this, 0, 0 );
        }

        /**
         * @brief Function Operator
         *
         * @param col Column index
         * @param row Row index
         * @param plane Plane index
         * @return Pixel value at the specified location
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
         * @brief Pre-Rasterize Method
         *
         * @param bbox Bounding box to pre-rasterize
         * @return prerasterize_type Pre-rasterized view
         */
        typedef Sparse_Image_View<ImageT,BlobT> prerasterize_type;
        prerasterize_type prerasterize( [[maybe_unused]] const math::Rect2i& bbox ) const
        {
            return (*this);
        }

        /**
         * @brief Rasterize Method
         *
         * @param dest Destination image
         * @param bbox Bounding box to rasterize
         */
        template <typename DestImageT>
        void rasterize( const DestImageT&   dest,
                        const math::Rect2i& bbox ) const
        {
            ops::rasterize( prerasterize( bbox ),
                            dest,
                            bbox );
        }

    private:

        /// @brief Underlying Image
        ImageT m_under_image;

        /// @brief Pixel Data
        std::deque<std::shared_ptr<BlobT>> m_blobs;
}; // End of Sparse_Image_View Class

} // End of tmns::image::ops::blob namespace