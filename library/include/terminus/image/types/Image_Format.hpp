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
 * @file    Image_Format.hpp
 * @author  Marvin Smith
 * @date    7/13/2023
*/
#pragma once

/// Terminus Libraries
#include "../pixel/Channel_Type_Enum.hpp"
#include "../pixel/Pixel_Format_Enum.hpp"

/// Other Terminus Libraries
#include <terminus/math/Rectangle.hpp>

namespace tmns::image {

class Image_Format
{
    public:

        /**
         * Default Constructor
        */
        Image_Format() = default;

        /**
         * Parameterized Constructor
         * @param cols Image columns
         * @param rows Image rows
         * @param channels Image channels
         * @param pixel_type Pixel format
         * @param channel_type
        */
        Image_Format( size_t            cols,
                      size_t            rows,
                      size_t            planes,
                      Pixel_Format_Enum pixel_type,
                      Channel_Type_Enum channel_type,
                      bool              premultiply );

        /**
         * Get the number of image columns
        */
        size_t cols() const;

        /**
         * Set the number of image columns
        */
        void set_cols( size_t cols );

        /**
         * Get the number of image rows
        */
        size_t rows() const;

        /**
         * Set the nubmer of image rows
        */
        void set_rows( size_t rows );

        /**
         * Get an image bounding box
        */
        math::Rect2i bbox() const;

        /**
         * Get the number of image planes
        */
        size_t planes() const;

        /**
         * Set the number of image planes
        */
        void set_planes( size_t planes );

        /**
         * Get the image pixel type
        */
        Pixel_Format_Enum pixel_type() const;

        /**
         * Set the image pixel type
        */
        void set_pixel_type( Pixel_Format_Enum tp );

        /**
         * Get the image channel type
        */
        Channel_Type_Enum channel_type() const;

        /**
         * Set the image channel type
        */
        void set_channel_type( Channel_Type_Enum tp );

        /**
         * Get the number of channels
        */
        int channels() const;

        /**
         * Check if we should apply alpha channel adjustments
        */
        bool premultiply() const;

        /**
         * Check if the data is fully structured.
        */
        bool complete() const;

        /**
         * Get the column stride.  Note this does no error or bounds checking.
         * Do not use this until the format class is fully initialized.
        */
        size_t cstride  () const;

        /**
         * Get the row stride.  Note this does no error or bounds checking.
         * Do not use this until the format class is fully initialized.
        */
        size_t rstride  () const;

        /**
         * Get the plane stride.  Note this does no error or bounds checking.
         * Do not use this until the format class is fully initialized.
        */
        size_t pstride  () const;

        /**
         * Get the raster size in bytes.  Note this does no error or bounds
         * checking.  Do not use this until the format class is fully
         * initialized
        */
        size_t raster_size_bytes() const;


        /**
         * Print to log-friendly string
        */
        std::string to_string( size_t offset = 0 ) const;

        /**
         * Check if we can convert the two image format types.
        */
        static Result<void> can_convert( const Image_Format&  dest,
                                         const Image_Format&  source );

    private:

        /// Image Cols
        size_t m_cols { 0 };

        /// Image Rows
        size_t m_rows { 0 };

        /// Planes
        size_t m_planes { 0 };

        /// Pixel Format
        Pixel_Format_Enum m_pixel_type { Pixel_Format_Enum::UNKNOWN };

        /// Channel-Type
        Channel_Type_Enum m_channel_type { Channel_Type_Enum::UNKNOWN };

        /// Premultiply
        bool m_premultiply { true };

}; // End of Image_Format Class

} // End of tmns::image namespace