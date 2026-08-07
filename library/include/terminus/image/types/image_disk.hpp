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
 * @file    Image_Disk.hpp
 * @author  Marvin Smith
 * @date    7/20/2023
*/
#pragma once

// Terminus Image Libraries
#include <terminus/image/io/image_resource_disk.hpp>
#include <terminus/image/types/image_base.hpp>
#include <terminus/image/types/image_resource_base.hpp>
#include <terminus/image/types/image_resource_view.hpp>
#include <terminus/image/utility/log_utilities.hpp>

namespace tmns::image {

/**
 * Class for "lazy" evaluation of image data.  Prevents loading too much data into RAM and
 * instead, only "rasterizes" blocks of data when requested.
*/
template <typename PixelT>
class Image_Disk : public Image_Base<Image_Disk<PixelT>>
{
    public:

        /// Image-Type providing block-processing capabilities
        typedef ops::Block_Rasterize_View<Image_Resource_View<PixelT> > impl_type;

        /// Pixel Type
        typedef typename impl_type::pixel_type    pixel_type;

        /// Return type when you query actual data
        typedef typename impl_type::result_type  result_type;

        /// Base type of the image
        typedef Image_Base<Image_Disk<PixelT>> base_type;

        /// Pixel Iterator Type
        typedef typename impl_type::pixel_accessor pixel_accessor;

        Image_Disk( io::Image_Resource_Disk::ptr_t   resource,
                    core::cache::Cache_Local::ptr_t  cache )
          : m_resource( resource ),
            m_impl( resource,
                    m_resource->block_read_size(),
                    1,
                    cache )
        {
            this->metadata()->insert( resource->metadata(),
                                      true );
        }

        /**
         * Destructor
        */
        ~Image_Disk() = default;

        /**
         * Get the number of columns
         */
        int cols() const
        {
            return m_impl.cols();
        }

        /**
         * Get the number of rows.
         */
        int rows() const
        {
            return m_impl.rows();
        }

        /**
         * Get the number of planes
         */
        int planes() const
        {
            return m_impl.planes();
        }

        /**
         * Get the position of the origin
        */
        pixel_accessor origin() const
        {
            return m_impl.origin();
        }

        result_type operator()( int x,
                                int y,
                                int p = 0 ) const
        {
            return m_impl(x,y,p);
        }

        /**
         * Prerasterize the image
        */
        typedef typename impl_type::prerasterize_type prerasterize_type;
        prerasterize_type prerasterize( const math::Rect2i& bbox ) const
        {
            return m_impl.prerasterize( bbox );
        }

        /**
         * Rasterize the image
        */
        template <class DestT>
        void rasterize( const DestT&        dest,
                        const math::Rect2i& bbox ) const
        {
            tmns::log::trace( LOG_IMAGE_TAG(), "start of rasterize. bbox: ",
                                               bbox.to_string()  );
            m_impl.rasterize( dest, bbox );
            tmns::log::trace( LOG_IMAGE_TAG(), "end of rasterize" );
        }

        /**
         * Get the image filename
        */
        std::filesystem::path pathname() const
        {
            return m_resource->pathname();
        }

        /**
         * Get this class name
        */
        static std::string class_name()
        {
            return "Image_Disk";
        }

        static std::string full_name()
        {
            return class_name() + "<" + math::Compound_Name<pixel_type>::name() + ">";
        }

    private:

        // Underlying
        std::shared_ptr<io::Image_Resource_Disk> m_resource;

        /// Underlying block resource
        impl_type m_impl;


}; // End of Image_Disk Class

} // end of tmns::image namespace