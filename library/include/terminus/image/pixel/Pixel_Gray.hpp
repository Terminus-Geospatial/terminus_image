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
 * @file    Pixel_Gray.hpp
 * @author  Marvin Smith
 * @date    7/29/2023
*/
#pragma once

// Terminus Image Libraries
#include <terminus/image/pixel/Channel_Range.hpp>
#include <terminus/image/pixel/Pixel_Base.hpp>
#include <terminus/math/types/Compound_Types.hpp>

namespace tmns {
namespace image {

/**
 * Grayscale Pixeltype
*/
template <typename ChannelT>
class Pixel_Gray : public Pixel_Base<Pixel_Gray<ChannelT>>
{
    public:

        typedef ChannelT channel_type;

        /**
         * Default Constructor
        */
        Pixel_Gray() = default;

        /**
         * Set all channels to the same luminance value
        */
        Pixel_Gray( channel_type pix )
          : m_data( pix )
        {}

        /**
         * Conversion from other channel-type
         */
        template <typename OtherChT>
        Pixel_Gray( const OtherChT& other )
        {
            m_data = ChannelT( other );
        }

        /**
         * Downcast to the raw channel-type
        */
        operator ChannelT() const
        {
            return m_data;
        }

        /**
         * Indexing Operators
        */
        const channel_type& operator[]( size_t idx ) const
        {
            return m_data;
        }

        /**
         * Indexing Operators
        */
        channel_type& operator[]( size_t idx )
        {
            return m_data;
        }

        /// Number of channels
        static constexpr int NUM_CHANNELS = 1;

        static std::string class_name()
        {
            return "Pixel_Gray<" + ChannelT::name() + ">";
        }

    private:

        /// Underlying Pixel Data
        channel_type m_data{ 0 };

}; // End of Pixel_Gray Class

} // End of image namespace

namespace math {
/**
 * Per Fundamental_Types.hpp, we need to register our
 * compound types.  See header for details
*/
template <typename ChannelT>
struct Compound_Channel_Type<image::Pixel_Gray<ChannelT> >
{
    typedef ChannelT type;
}; // End of Compound_Channel_Type<> Struct

template <typename ChannelT>
struct Compound_Channel_Count<image::Pixel_Gray<ChannelT>>
{
    static const size_t value = image::Pixel_Gray<ChannelT>::NUM_CHANNELS;
}; // End of Compound_Channel_Count<> Struct

template <typename OldChannelT, typename NewChannelT>
struct Compound_Channel_Cast<image::Pixel_Gray<OldChannelT>, NewChannelT>
{
    typedef image::Pixel_Gray<NewChannelT> type;
}; // End of Compound_Channel_Cast<> structure

template <typename OldChannelT, typename NewChannelT>
struct Compound_Channel_Cast<image::Pixel_Gray<OldChannelT>, const NewChannelT>
{
    typedef const image::Pixel_Gray<NewChannelT> type;
}; // End of Compound_Channel_Cast<> structure

} // End of math namespace

namespace image {

/// Aliases for easier typing
using PixelGray_u8  = Pixel_Gray<uint8_t>;
using PixelGray_u16 = Pixel_Gray<uint16_t>;

using PixelGray_f32  = Pixel_Gray<float>;
using PixelGray_f64  = Pixel_Gray<double>;

} // End of image namespace
} // End of tmns  namespace