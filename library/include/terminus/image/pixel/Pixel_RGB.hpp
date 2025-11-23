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
 * @file    Pixel_RGB.hpp
 * @author  Marvin Smith
 * @date    7/11/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/image/pixel/Channel_Range.hpp>
#include <terminus/image/pixel/Pixel_Base.hpp>
#include <terminus/math/types/Compound_Types.hpp>

namespace tmns {
namespace image {

/**
 * RGB PixelType
*/
template <typename ChannelT>
class Pixel_RGB : public Pixel_Base<Pixel_RGB<ChannelT>>
{
    public:

        typedef ChannelT channel_type;

        /**
         * Default Constructor
        */
        Pixel_RGB() = default;

        /**
         * Set all channels to the same luminance value
        */
        Pixel_RGB( const channel_type& pix )
        {
            m_data.fill( pix );
        }

        /**
         * Set all channels independently
        */
        Pixel_RGB( const channel_type& r,
                   const channel_type& g,
                   const channel_type& b )
        {
            m_data[0] = r;
            m_data[1] = g;
            m_data[2] = b;
        }

        /**
         * Constructor
        */
        template <typename OtherChannelT>
        Pixel_RGB( const Pixel_RGB<OtherChannelT>& other )
        {
            m_data[0] = ChannelT( other[0] );
            m_data[1] = ChannelT( other[1] );
            m_data[2] = ChannelT( other[2] );
        }

        /**
         * Get the pixel at the specified location
        */
        const channel_type& operator[]( size_t idx ) const
        {
            return m_data[idx];
        }

        /**
         * Get the pixel reference at the specified location
        */
        channel_type& operator[]( size_t idx )
        {
            return m_data[idx];
        }

        /**
         * Send an all white pixel back
        */
        static Pixel_RGB<ChannelT> max()
        {
            return Pixel_RGB<ChannelT>( Channel_Range<ChannelT>::max() );
        }

        static std::string class_name()
        {
            return "Pixel_RGB<" + math::Data_Type_Name<ChannelT>::name() + ">";
        }

        /**
         * Return a "red" pixel where R is max
         */
        static Pixel_RGB<ChannelT> red()
        {
            return Pixel_RGB<ChannelT>( Channel_Range<ChannelT>::max(),
                                        Channel_Range<ChannelT>::min(),
                                        Channel_Range<ChannelT>::min() );
        }

        /**
         * Return a "green" pixel where 2nd channel is max
         */
        static Pixel_RGB<ChannelT> green()
        {
            return Pixel_RGB<ChannelT>( Channel_Range<ChannelT>::min(),
                                        Channel_Range<ChannelT>::max(),
                                        Channel_Range<ChannelT>::min() );
        }

        /**
         * Return a "blue" pixel where 3rd channel is max
         */
        static Pixel_RGB<ChannelT> blue()
        {
            return Pixel_RGB<ChannelT>( Channel_Range<ChannelT>::min(),
                                        Channel_Range<ChannelT>::min(),
                                        Channel_Range<ChannelT>::max() );
        }

        std::string to_string() const
        {
            std::stringstream sout;
            sout << class_name() << "( r:" << m_data[0]
                 << ", g: " << m_data[1] << ", b: " << m_data[2] << ")";
            return sout.str();
        }

        /// Number of channels
        static constexpr int NUM_CHANNELS = 3;

    private:

        /// Underlying Pixel Data
        std::array<channel_type,3> m_data{ 0, 0, 0 };

}; // End of Pixel_RGB Class

} // End of image namespace

namespace math {

/**
 * Per Fundamental_Types.hpp, we need to register our
 * compound types.  See header for details
*/
template <typename ChannelT>
struct Compound_Channel_Type<image::Pixel_RGB<ChannelT> >
{
    typedef ChannelT type;
}; // End of Compound_Channel_Type<> Struct

template <typename ChannelT>
struct Compound_Channel_Count<image::Pixel_RGB<ChannelT>>
{
    static const size_t value = image::Pixel_RGB<ChannelT>::NUM_CHANNELS;
}; // End of Compound_Channel_Count<> Struct

template <typename OldChannelT, typename NewChannelT>
struct Compound_Channel_Cast<image::Pixel_RGB<OldChannelT>, NewChannelT>
{
    typedef image::Pixel_RGB<NewChannelT> type;
}; // End of Compound_Channel_Cast<> structure

template <typename OldChannelT, typename NewChannelT>
struct Compound_Channel_Cast<image::Pixel_RGB<OldChannelT>, const NewChannelT>
{
    typedef const image::Pixel_RGB<NewChannelT> type;
}; // End of Compound_Channel_Cast<> structure

} // End of math namespace

namespace image {

/// Aliases for easier typing
using PixelRGB_u8  = Pixel_RGB<uint8_t>;
using PixelRGB_u16 = Pixel_RGB<uint16_t>;

using PixelRGB_f32  = Pixel_RGB<float>;
using PixelRGB_f64  = Pixel_RGB<double>;

} // End of image namespace
} // End of tmns namespace