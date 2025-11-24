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
 * @file    Block_Generator.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// Terminus Image Libraries
#include "../../types/Image_Memory.hpp"

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>


namespace tmns::image::ops::block {


template <typename ImageT>
class Block_Generator
{
    public:

        typedef Image_Memory<typename ImageT::pixel_type> value_type;

        /**
         * Constructor
        */
        Block_Generator( const std::shared_ptr<ImageT>& child,
                         const math::Rect2i&            bbox )
          : m_child( child ),
            m_bbox( bbox )
        {}

        static std::string class_name()
        {
            return "Block_Generator";
        }

        /**
         * Return the size of the image in bytes that the image
         * occupies.
         */
        size_t size_bytes() const
        {
            return m_bbox.width() * m_bbox.height() * m_child->planes() * sizeof( typename ImageT::pixel_type );
        }

        /**
         * Rasterize the image chunk into memory from wherever it is derived from
         */
        std::shared_ptr<value_type> generate() const
        {
            auto ptr = std::shared_ptr<value_type>( new value_type(  m_bbox.width(), m_bbox.height(), m_child->planes() ) );
            m_child->rasterize( *ptr, m_bbox );
            return ptr;
        }

    private:

        /// Pointer back to source image
        std::shared_ptr<ImageT> m_child { nullptr };

        /// ROI of input image
        math::Rect2i m_bbox;

}; // End of Block_Generator Class

} // End of tmns::image::ops::block namespace