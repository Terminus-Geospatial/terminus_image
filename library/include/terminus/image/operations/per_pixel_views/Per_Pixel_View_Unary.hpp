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
 * @file    image_casts.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// Terminus Image Libraries
#include "../../types/Image_Base.hpp"
#include "../../types/Image_Traits.hpp"
#include "../rasterize.hpp"
#include "Per_Pixel_Accessor_Unary.hpp"

// C++ Libraries
#include <type_traits>

namespace tmns::image {
namespace ops {

/**
 * Class which applies the input functor on every pixel
*/
template <typename ImageT,
          typename FunctorT>
class Per_Pixel_View_Unary : public Image_Base<Per_Pixel_View_Unary<ImageT,FunctorT> >
{
    public:

        /// Result Type
        typedef typename std::invoke_result<FunctorT,typename ImageT::pixel_type>::type             result_type;

        /// Pixel Type
        typedef typename std::remove_cv<typename std::remove_reference<result_type>::type>::type     pixel_type;

        /// Pixel Iterator Type
        typedef Per_Pixel_Accessor_Unary<typename ImageT::pixel_accessor, FunctorT>         pixel_accessor;

        /**
         * Constructor
         * @param image
        */
        Per_Pixel_View_Unary( const ImageT& image )
          : m_image( image ){}

        /**
         * Constructor
         * @param image
         * @param func
         */
        Per_Pixel_View_Unary( const ImageT&   image,
                              const FunctorT& func )
          : m_image( image ),
            m_func( func )
        {
        }

        /**
         * Get image columns
        */
        size_t cols() const
        {
            return m_image.cols();
        }

        /**
         * Get image rows
        */
        size_t rows() const
        {
            return m_image.rows();
        }

        /**
         * Get image planes
         */
        size_t planes() const
        {
            return m_image.planes();
        }

        /**
         * Get the origin of the image
         */
        pixel_accessor origin() const
        {
            return pixel_accessor( m_image.origin(),
                                   m_func );
        }

        /**
         * Apply the functor on a single pixel
         */
        result_type operator()( size_t x, size_t y, size_t p = 0 ) const
        {
            return m_func( m_image( x, y, p ) );
        }

        /**
         * Re-assign the image.
         */
        template <typename SourceT>
        Per_Pixel_View_Unary&  operator = ( const Image_Base<SourceT>& new_image )
        {
            new_image.impl().rasterize( *this,
                                        new_image.full_bbox() );
            return (*this);
        }

        /**
         * Pre-reasterize
        */
        typedef Per_Pixel_View_Unary<typename ImageT::prerasterize_type, FunctorT> prerasterize_type;
        prerasterize_type prerasterize( const math::Rect2i& bbox ) const
        {
            return prerasterize_type( m_image.prerasterize( bbox ),
                                      m_func );
        }

        /**
         * Rasterize the image
        */
       template <typename DestT>
       void rasterize( const DestT&        dest,
                       const math::Rect2i& bbox ) const
        {
            ops::rasterize( prerasterize( bbox ),
                            dest,
                            bbox );
        }

    private:

        /// Image to modify
        ImageT m_image;

        /// Function to apply on image
        FunctorT  m_func;
}; // End of Per_Pixel_View_Unary class

} // End of ops namespace

/**
 * Allow multiplication
*/
template <typename ImageT,
          typename FunctorT>
struct Is_Multiply_Accessible<ops::Per_Pixel_View_Unary<ImageT,FunctorT>>
       : std::is_reference<typename ops::Per_Pixel_View_Unary<ImageT,FunctorT>::result_type>::type {};


} // End of tmns::image namespace