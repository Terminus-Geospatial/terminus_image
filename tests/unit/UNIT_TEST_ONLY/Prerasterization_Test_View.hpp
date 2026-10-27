/**
 * @file    Prerasterization_Test_View.hpp
 * @author  Marvin Smith
 * @date    7/29/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/image/types/image_base.hpp>
#include <terminus/image/types/image_memory.hpp>
#include <terminus/math/rectangle.hpp>

// C++ Libraries
#include <algorithm>
#include <memory>
#include <stdexcept>

namespace tx = tmns::image;

/**
 * Test View for calling the pre-rasterize method
*/
template <typename PixelT>
class Prerasterization_Test_View : public tx::Image_Base<Prerasterization_Test_View<PixelT>>
{
    public:

        /// Underlying Pixel Type
        typedef typename tx::Image_Memory<PixelT>::pixel_type pixel_type;

        /// Pixel Result for () calls
        typedef typename tx::Image_Memory<PixelT>::result_type result_type;

        /// Pixel Accessor Type
        typedef typename tx::Image_Memory<PixelT>::pixel_accessor pixel_accessor;

        /**
         * Constructor
        */
        Prerasterization_Test_View( int cols,
                                    int rows )
             : m_image( std::make_shared<tx::Image_Memory<uint8_t>>( cols, rows ) )
        {
            std::fill( m_image->data(),
                       m_image->data() + m_image->cols() * m_image->rows(),
                       static_cast<uint8_t>( 0 ) );
        }



        /**
         * Get the image columns
         */
        int cols() const
        {
            return m_image->cols();
        }

        /**
         * Get the image rows
         */
        int rows() const
        {
            return m_image->rows();
        }

        /**
         * @brief Get the image planes
         */
        int planes() const
        {
            return 1;
        }

        /**
         * @brief Get the origin
         */
        pixel_accessor origin() const
        {
            return m_image->origin();
        }

        /**
         * @brief Get the image bounding box
        */
        tmns::math::Rect2i bbox() const
        {
            bool result_set = false;
            tmns::math::Rect2i result;
            for( int y = 0; y < m_image->rows(); ++y ){
            for( int x = 0; x < m_image->cols(); ++x ){

                if( (*m_image)( x, y ) > 0 )
                {
                    if( !result_set )
                    {
                        result = tmns::math::Rect2i( x, y, 0, 0 );
                        result_set = true;
                    }
                    else
                    {
                        result = tmns::math::Rect2i::set_union( result, tmns::math::ToPoint2<int>( x, y ) );
                    }
                }
            }}
            if( result.width() > 0 && result.height() > 0 )
            {
                result.set_max( result.max() + tmns::math::ToPoint2<int>( 1, 1 ) );
            }
            return result;
        }

        /**
         * @brief Prerasterize the image
         * @param bbox The bounding box to prerasterize
         * @return The prerasterized view
         */
        typedef Prerasterization_Test_View<PixelT> prerasterize_type;
        prerasterize_type prerasterize( const tmns::math::Rect2i& bbox ) const
        {
            if( bbox.min().x() < 0 ||
                bbox.min().y() < 0 ||
                bbox.max().x() > m_image->cols() ||
                bbox.max().y() > m_image->rows() )
            {
                throw std::runtime_error( class_name() + "::prerasterize() called with bounding box that exceeds image dimensions" );
            }
            for( int y= bbox.min().y(); y<bbox.max().y(); ++y ) {
            for( int x= bbox.min().x(); x<bbox.max().x(); ++x ) {
                (*m_image)(x,y) = 1;
            }}
            return *this;
        }

        /**
         * @brief Rasterize the image
         * @param dest The destination to rasterize to
         * @param bbox The bounding box to rasterize
         */
        template <class DestT>
        void rasterize( const DestT&              dest,
                        const tmns::math::Rect2i& bbox ) const
        {
            tx::ops::rasterize( prerasterize( bbox ), dest, bbox );
        }

        /**
         * @brief Get the class name
         * @return The class name
         */
        static std::string class_name()
        {
            return "Prerasterization_Test_View";
        }

    private:

        // Underlying image
        std::shared_ptr<tx::Image_Memory<uint8_t>> m_image;
};