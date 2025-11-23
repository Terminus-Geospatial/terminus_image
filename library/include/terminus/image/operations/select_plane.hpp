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
 * @file    select_plane.hpp
 * @author  Marvin Smith
 * @date    7/28/2023
*/
#pragma once

// Terminus Image Libraries
#include "rasterize.hpp"

namespace tmns::image::ops {

/**
 * Return a single plane from a multi-plane image
 */
template <class ImageT>
class Select_Plane_View : public Image_Base<Select_Plane_View<ImageT> >
{
    public:

        /// @brief Underlying Pixel Type
        typedef typename ImageT::pixel_type pixel_type;

        /// @brief Result from a pixel operation
        typedef typename ImageT::result_type result_type;

        /// @brief Iterator type
        typedef typename ImageT::pixel_accessor pixel_accessor;

        /**
         * Constructor
        */
        Select_Plane_View( const ImageT& image,
                           int           plane )
          : m_child( image ),
            m_plane( plane )
        {
        }

        /**
         * Get the image columns
        */
        size_t cols() const
        {
            return m_child.cols();
        }

        /**
         * Get the image rows
        */
        size_t rows() const
        {
            return m_child.rows();
        }

        /**
         * Get the image planes
        */
        size_t planes() const
        {
            return 1;
        }

        /**
         * Get the buffer origin
        */
        pixel_accessor origin() const
        {
            return m_child.origin().advance( 0, 0, m_plane );
        }

        /**
         * Function Operator
        */
        result_type operator()( size_t i, size_t j, size_t p = 0 ) const
        {
            return m_child( i, j, m_plane + p );
        }

        /**
         * Assignment (Reference) Operator
        */
        template <class InputImageT>
        const Select_Plane_View& operator = ( const Image_Base<InputImageT>& image ) const
        {
            image.impl().rasterize( *this,
                                    math::Rect2i( 0,
                                                  0,
                                                  image.impl().cols(),
                                                  image.impl().rows() ) );
            return *this;
        }

        /**
         * Assignment (Copy) Operator
        */
        template <class InputImageT>
        Select_Plane_View&  operator = ( const Image_Base<InputImageT>& image )
        {
            *const_cast<const Select_Plane_View*>(this) = image.impl();
            return *this;
        }

        /**
         * Pre-rasterize image
        */
        typedef Select_Plane_View<typename ImageT::prerasterize_type> prerasterize_type;
        prerasterize_type prerasterize( const math::Rect2i& bbox ) const
        {
            return prerasterize_type( m_child.prerasterize( bbox ),
                                      m_plane );
        }

        /**
         * Rasterize image
        */
        template <class DestT>
        void rasterize( const DestT&        dest,
                        const math::Rect2i& bbox ) const
        {
            ops::rasterize( prerasterize(bbox), dest, bbox );
        }

    private:

        // Child Image
        ImageT m_child;

        // Plane to extract
        int m_plane;
};

/**
 * Extracts a single plane of a multi-plane image.  This function returns a
 * writeable view of a single plane of a multi-plane \see vw::Select_Plane_View
 */
template <class ImageT>
Select_Plane_View<ImageT> select_plane( const Image_Base<ImageT>& image,
                                        int                       plane )
{
    return Select_Plane_View<ImageT>( image.impl(), plane );
}

} // end of tmns::image::ops namespace