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
 * @file    Image_Resource_View.hpp
 * @author  Marvin Smith
 * @date    7/22/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/concurrency/Mutex.hpp>
#include <terminus/image/io/read_image.hpp>
#include <terminus/image/operations/block/Block_Rasterize_View.hpp>
#include <terminus/image/operations/crop_image.hpp>
#include <terminus/image/pixel/Pixel_Accessor_Loose.hpp>
#include <terminus/image/pixel/Pixel_Format_Enum.hpp>
#include <terminus/image/pixel/Pixel_Format_ID.hpp>
#include <terminus/image/types/Image_Memory.hpp>
#include <terminus/image/types/Image_Resource_Base.hpp>
#include <terminus/math/types/Fundamental_Types.hpp>
#include <terminus/outcome/Result.hpp>

namespace tmns::image {

/**
 * Class which wraps image resources to make it easier to directly
 * access pixel data.
*/
template <typename PixelT>
class Image_Resource_View : public Image_Base<Image_Resource_View<PixelT>>
{
    public:

        typedef Image_Resource_View<PixelT> m_ptr;

        /// Base Type of the image
        typedef Image_Base<Image_Resource_View<PixelT>> base_type;

        /// Pixel Type of the Image
        typedef PixelT pixel_type;

        /// Pixel type returned (Note we don't return by reference
        // like other types with their own internal memory, since ours
        // is owned by the resource )
        typedef PixelT result_type;

        /// The view's pixel accessor type.
        typedef Pixel_Accessor_Loose<Image_Resource_View<PixelT> > pixel_accessor;

        /**
         * Build the view from a given resource
        */
        Image_Resource_View( Read_Image_Resource_Base::ptr_t resource )
          : m_resource( resource ),
            m_planes( m_resource->planes() )
        {
            m_constructor_status = initialize();
        }

        /**
         * Default Destructor
        */
        ~Image_Resource_View() = default;

        /**
         * Get the construction status
        */
        Result<void> get_constructor_status() const
        {
            return m_constructor_status;
        }

        /**
         * Get Image Cols
        */
        inline size_t cols() const { return m_resource->cols(); }

        /**
         * Get Image Rows
         */
        inline size_t rows() const { return m_resource->rows(); }

        /**
         * Get image planes
         */
        inline size_t planes() const { return m_planes; }

        /**
         * Return the pixel at the specified location.
         * @note:  This is a slow process and should not be used.
        */
        result_type operator() ( int x, int y, int plane = 0 ) const
        {
            // Lock the mutex
            core::conc::Mutex::Lock lck( m_resource_mtx );

            // Create output image memory object
            Image_Memory<PixelT> dest_image( 1, 1, m_planes );
            io::read_image( dest_image, 
                            m_resource,
                            math::Rect2i( x, y, 1, 1 ) );
            return dest_image( 0, 0, plane );
        }

        /**
         * Get a pointer back to the origin
        */
        pixel_accessor origin() const
        {
            return pixel_accessor( *this, 0, 0 );
        }

        /**
         * Get the original resource
        */
        const Image_Resource_Base* resource() const
        {
            return dynamic_cast<const Image_Resource_Base*>( m_resource.get() );
        }

        /**
         * Pre-Rasterize
        */
        typedef ops::Crop_View<Image_Memory<pixel_type> > prerasterize_type;
        prerasterize_type prerasterize( const math::Rect2i& bbox ) const
        {
            Image_Memory<PixelT> buffer( bbox.width(),
                                         bbox.height() );
            this->rasterize( buffer, bbox );
            return Crop_View<Image_Memory<PixelT> >( buffer,
                                                     math::Rect2i( -bbox.min().x(),
                                                                   -bbox.min().y(),
                                                                   cols(),
                                                                   rows() ) );
        }

        /**
         * Rasterize the view
        */
        template <typename DestT>
        void rasterize( const DestT&         dest,
                        const math::Rect2i&  bbox ) const
        {
            core::conc::Mutex::Lock lock( m_resource_mtx );
            //m_resource->read( dest.buffer(), bbox );
            io::read_image( dest, m_resource, bbox );
        }

    private:

        /**
         * Determine the number of planes for the image based on the
         * resource and your desired destination pixel type.
        */
        Result<void> initialize()
        {
            // If the user has requested a multi-channel pixel type, but the
            // file is a multi-plane, scalar-pixel file, we force a single-plane interpretation.
            if( math::Compound_Channel_Count<PixelT>::value > 1 &&
                m_resource->pixel_type() == Pixel_Format_Enum::SCALAR )
            {
                m_planes = 1;
            }

            // On the other hand, the user has requested a scalar pixel type
            // but the file has a multi-channel pixel type, then we force a
            // multi-plane interpretation.
            if( math::Is_Scalar<PixelT>::value    &&
                m_resource->channels() >= 1 &&
                m_resource->planes()   == 1 )
            {
                m_planes = m_resource->channels();
            }

            return outcome::ok();
        }

        /// Image Resource Data
        Read_Image_Resource_Base::ptr_t m_resource;

        /// Mutex lock for hitting the resource
        mutable core::conc::Mutex m_resource_mtx;

        /// Number of image planes
        int m_planes { 0 };

        /// Load Status (Created after constructor runs)
        Result<void> m_constructor_status { tmns::outcome::ok() };

}; // End of Image_Resource_View Class

} // End of tmns::image namespace