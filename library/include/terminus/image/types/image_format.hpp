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
#include <terminus/image/pixel/channel_type_enum.hpp>
#include <terminus/image/pixel/pixel_format_enum.hpp>

/// Other Terminus Libraries
#include <terminus/math/rectangle.hpp>

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
        Image_Format( int               cols,
                      int               rows,
                      int               planes,
                      Pixel_Format_Enum pixel_type,
                      Channel_Type_Enum channel_type,
                      bool              premultiply );

        /**
         * @brief Get the number of image columns
         *
         * @return int Number of image columns
         */
        int cols() const;

        /**
         * @brief Set the number of image columns
         *
         * @param cols Number of image columns
         */
        void set_cols( int cols );

        /**
         * @brief Get the number of image rows
         *
         * @return int Number of image rows
         */
        int rows() const;

        /**
         * @brief Set the number of image rows
         *
         * @param rows Number of image rows
         */
        void set_rows( int rows );

        /**
         * @brief Get the number of image planes
         *
         * @return int Number of image planes
         */
        int planes() const;

        /**
         * @brief Set the number of image planes
         *
         * @param planes Number of image planes
         */
        void set_planes( int planes );

        /**
         * @brief Get an image bounding box
         *
         * @return math::Rect2i Bounding box
         */
        math::Rect2i bbox() const;

        /**
         * @brief Get the image pixel type
         *
         * @return Pixel_Format_Enum Pixel type
         */
        Pixel_Format_Enum pixel_type() const;

        /**
         * @brief Set the image pixel type
         *
         * @param tp Pixel type
         */
        void set_pixel_type( Pixel_Format_Enum tp );

        /**
         * @brief Get the image channel type
         *
         * @return Channel_Type_Enum Channel type
         */
        Channel_Type_Enum channel_type() const;

        /**
         * @brief Set the image channel type
         *
         * @param tp Channel type
         */
        void set_channel_type( Channel_Type_Enum tp );

        /**
         * @brief Get the number of channels
         *
         * @return int Number of channels
         */
        int channels() const;

        /**
         * @brief Check if we should apply alpha channel adjustments
         */
        bool premultiply() const;

        /**
         * @brief Check if the data is fully structured
         */
        bool complete() const;

        /**
         * @brief Get the column stride.  Note this does no error or bounds checking.
         * Do not use this until the format class is fully initialized.
         */
        ssize_t cstride() const;

        /**
         * @brief Get the row stride.  Note this does no error or bounds checking.
         * Do not use this until the format class is fully initialized.
         */
        ssize_t rstride() const;

        /**
         * @brief Get the plane stride.  Note this does no error or bounds checking.
         * Do not use this until the format class is fully initialized.
         *
         * @return ssize_t Plane stride
         */
        ssize_t pstride() const;

        /**
         * @brief Get the raster size in bytes.  Note this does no error or bounds
         * checking.  Do not use this until the format class is fully
         * initialized
         *
         * @return size_t Raster size in bytes
         */
        size_t raster_size_bytes() const;


        /**
         * @brief Print to log-friendly string
         *
         * @param offset Offset to add to the string
         * @return std::string Log-friendly string
         */
        std::string to_string( size_t offset = 0 ) const;

        /**
         * @brief Check if we can convert the two image format types.
         *
         * @param dest Destination image format
         * @param source Source image format
         * @return Result<void> Result of the conversion check
         */
        static Result<void> can_convert( const Image_Format&  dest,
                                         const Image_Format&  source );

    private:

        /// @brief Image Cols
        int m_cols { 0 };

        /// @brief Image Rows
        int m_rows { 0 };

        /// @brief Number of planes
        int m_planes { 0 };

        /// @brief Pixel Format
        Pixel_Format_Enum m_pixel_type { Pixel_Format_Enum::UNKNOWN };

        /// @brief Channel-Type
        Channel_Type_Enum m_channel_type { Channel_Type_Enum::UNKNOWN };

        /// @brief Premultiply
        bool m_premultiply { true };

}; // End of Image_Format Class

} // End of tmns::image namespace