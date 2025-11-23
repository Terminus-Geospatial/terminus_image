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
 * @file    CropView.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// C++ Libraries
#include <type_traits>

// Terminus Libraries
#include "../types/Image_Traits.hpp"
#include "rasterize.hpp"

namespace tmns::image {
namespace ops {

// Forward Declaration
template <typename ImageT>
class Crop_View;

} // End of ops namespace

/**
 * Allow indexing via floating-point indices if the child image-type is also indexible
*/
template <class ImageT>
struct Is_Floating_Point_Indexable<ops::Crop_View<ImageT> >
{
    static consteval bool value(){ return Is_Floating_Point_Indexable<ImageT>::value(); }
}; // End of Is_Floating_Point_Indexable<> struct

/**
 * Allow multiplication as long as parent type is multiplication-ready
*/
template <class ImageT>
struct Is_Multiply_Accessible<ops::Crop_View<ImageT> > : public Is_Multiply_Accessible<ImageT> {};

namespace ops {

/**
 * Image instance which performs cropping when rasterized
 */
template <typename ImageT>
class Crop_View : public Image_Base<Crop_View<ImageT>>
{
    public:

        /// @brief Pixel Type
        typedef typename ImageT::pixel_type pixel_type;

        /// @brief Result Type (Notice not a reference)
        typedef typename ImageT::result_type result_type;

        /// @brief  Pixel Access Method
        typedef typename ImageT::pixel_accessor pixel_accessor;

        /**
         * @brief If Floating-Point Indexible, use doubles, otherwise, stick with integer
        */
        typedef typename std::conditional_t<Is_Floating_Point_Indexable<ImageT>::value(), double, int> offset_type;

        /**
         * Constructor
        */
        Crop_View( const ImageT&     image,
                   const offset_type upper_left_i,
                   const offset_type upper_left_j,
                   const int         width,
                   const int         height )
          :  m_child( image ),
             m_ci( upper_left_i ),
             m_cj( upper_left_j ),
             m_di( width ),
             m_dj( height )
        {}

        /**
         * Constructor
        */
        template<class RealT>
        Crop_View( const ImageT&                   image,
                   const math::Rectangle<RealT,2>& bbox )
         : m_child( image ),
           m_ci( ( offset_type )( bbox.min()[0] ) ),
           m_cj( ( offset_type )( bbox.min()[1] ) ),
           m_di( int( .5 + ( bbox.width() ) ) ),
           m_dj( int( .5 + ( bbox.height() ) ) )
        {}

        /**
         * Get the number of image columns
        */
        size_t cols() const { return m_di; }

        /**
         * Get the number of image rows
        */
        size_t rows() const { return m_dj; }

        /**
         * Get the number of image planes
        */
        size_t planes() const { return m_child.planes(); }

        /**
         * Get the image data origin
        */
        pixel_accessor origin() const
        {
            return m_child.origin().advance(m_ci, m_cj);
        }

        /**
         * Access a single pixel
         */
        result_type operator()( offset_type i,
                                offset_type j,
                                size_t p = 0 ) const
        {
            return m_child(m_ci + i, m_cj + j, p);
        }

        /**
         * Assignment/Reference Operator
        */
        const Crop_View& operator = ( const Crop_View& image ) const
        {
            image.rasterize( *this, math::Rect2i( 0, 0, image.impl().cols(), image.impl().rows() ) );
            return *this;
        }

        /**
         * Assignment / Copy Operator
        */
        template <class ViewT>
        const Crop_View& operator = ( const Image_Base<ViewT>& image ) const
        {
            image.impl().rasterize( *this,
                                    math::Rect2i( 0, 0,
                                                  image.impl().cols(),
                                                  image.impl().rows() ) );
            return *this;
        }

        /**
         * Get the internal child image
         */
        const ImageT& child() const
        {
            return m_child;
        }

        // Pre-Rasterize
        typedef Crop_View<typename ImageT::prerasterize_type> prerasterize_type;
        prerasterize_type prerasterize( const math::Rect2i& bbox ) const
        {
            return prerasterize_type( m_child.prerasterize( bbox + math::Point2_<offset_type>( { m_ci, m_cj } ) ),
                                                            m_ci,
                                                            m_cj,
                                                            m_di,
                                                            m_dj );
        }

        /**
         * Rasterize
        */
        template <class DestT>
        void rasterize( const DestT&        dest,
                        const math::Rect2i& bbox ) const
        {
            // FIXME Warning: This does not respect floating-point offsets!
            tmns::image::ops::rasterize( prerasterize(bbox), dest, bbox );
        }

    private:

        /// Child Image
        ImageT m_child;

        offset_type m_ci;
        offset_type m_cj;
        int m_di; // Cropped Image Width
        int m_dj; // Cropped Image Height

};

} // End of tmns::image::ops namespace

/**
 * Crop an image
 */
template <typename ImageT>
ops::Crop_View<ImageT> crop_image( const Image_Base<ImageT>& image,
                                   size_t                    ulx,
                                   size_t                    uly,
                                   size_t                    width,
                                   size_t                    height )
{
    return ops::Crop_View<ImageT>( image.impl(),
                                   ulx,
                                   uly,
                                   width,
                                   height );
}

/**
 * Crop an image
*/
template <typename ImageT, typename BBoxT>
ops::Crop_View<ImageT> crop_image( const Image_Base<ImageT>&       image,
                                   const math::Rectangle<BBoxT,2>& bbox )
{
    return ops::Crop_View<ImageT>( image.impl(), bbox );
}

} // End of tmns::image namespace