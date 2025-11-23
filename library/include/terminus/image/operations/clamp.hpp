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
 * @file    clamp.hpp
 * @author  Marvin Smith
 * @date    8/26/2023
*/
#pragma once

// Terminus Image Libraries
#include "per_pixel_views/Per_Pixel_View_Unary.hpp"


namespace tmns::image::ops {

template <typename PixelT>
class Channel_Clamp_Functor: public Unary_Return_Same_Type
{
    public:

        typedef typename Compound_Channel_Type<PixelT>::type channel_type;

        Channel_Clamp_Functor( channel_type low,
                               channel_type high )
            : m_low(low),
              m_high(high)
        {
        }

        channel_type operator()( channel_type value ) const
        {
            if      (value > m_high) { return m_high; }
            else if (value < m_low ) { return m_low;  }
            else                     { return value;  }
        }
    
    private:
    
        channel_type m_low, m_high;
}; // End of Channel_Clamp_Functor class

/**
 * Clamp the values in an image to fall within the range [low,high].
*/
template <typename ImageT,
          typename LowT,
          typename HighT>
Per_Pixel_View_Unary<ImageT,Unary_Compound_Functor<Channel_Clamp_Functor<typename ImageT::pixel_type>, 
                                                   typename ImageT::pixel_type > >
    clamp( const Image_Base<ImageT>& image,
           LowT                      low,
           HighT                     high )
{
    typedef Unary_Compound_Functor<Channel_Clamp_Functor<typename ImageT::pixel_type>, 
                                   typename ImageT::pixel_type> func_type;
    func_type func( Channel_Clamp_Functor<typename ImageT::pixel_type>(low,high) );
    return Per_Pixel_View_Unary<ImageT,func_type>( image.impl(), func );
}

/**
 * Clamp the values in an image to fall within the range [0,high].
 * The low end of the range is actually determined by the
 * Channel_Range type trait but is generally zero.
 */
template <typename ImageT,
          typename HighT>
Per_Pixel_View_Unary<ImageT,Unary_Compound_Functor<Channel_Clamp_Functor<typename ImageT::pixel_type>,
                                                   typename ImageT::pixel_type> >
    clamp( const Image_Base<ImageT>& image, HighT high )
{
    // Define the functor, which wraps the clamp method.
    typedef Unary_Compound_Functor<Channel_Clamp_Functor<typename ImageT::pixel_type>,
                                   typename ImageT::pixel_type> func_type;

    // Establish the range (depends on pixel's channel-type)
    typedef Channel_Range<typename Compound_Channel_Type<typename ImageT::pixel_type>::type> range_type;
    typename Compound_Channel_Type<typename ImageT::pixel_type>::type min_val = range_type::min();
    func_type func( Channel_Clamp_Functor<typename ImageT::pixel_type>(min_val,high) );
    return Per_Pixel_View_Unary<ImageT,func_type>( image.impl(), func );
}


/**
 * Clamp the values in an image to fall within the range [min,max],
 * where min and max are determined by the ChannelRange type trait
 * and are generally equal to 0.0 and 1.0 for floating point types
 * and 0 and the largest positve value for integral types.
 */
template <typename ImageT>
Per_Pixel_View_Unary<ImageT,Unary_Compound_Functor<Channel_Clamp_Functor<typename ImageT::pixel_type>,
                                                   typename ImageT::pixel_type> >
    clamp( const Image_Base<ImageT>& image )
{
    typedef Unary_Compound_Functor<Channel_Clamp_Functor<typename ImageT::pixel_type>,
                                   typename ImageT::pixel_type> func_type;
    
    typedef Channel_Range<typename Compound_Channel_Type<typename ImageT::pixel_type>::type> range_type;
    
    typename Compound_Channel_Type<typename ImageT::pixel_type>::type min_val = range_type::min();
    typename Compound_Channel_Type<typename ImageT::pixel_type>::type max_val = range_type::max();

    func_type func( Channel_Clamp_Functor<typename ImageT::pixel_type>( min_val, max_val ) );
    
    return Per_Pixel_View_Unary<ImageT,func_type>( image.impl(), func );
}


} /// End of tmns::image::ops namespace