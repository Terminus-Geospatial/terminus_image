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
 * @file    normalize.hpp
 * @author  Marvin Smith
 * @date    8/26/2023
*/
#pragma once

// Terminus Libraries
#include "../pixel/Pixel_Cast_Utilities.hpp"
#include "per_pixel_views/Per_Pixel_View_Unary.hpp"
#include "statistics/channel_operations.hpp"

namespace tmns::image::ops {

/**
 * Normalizes pixel data to specified range.
*/
template <typename PixelT>
class Channel_Normalize_Functor: public math::Unary_Return_Same_Type
{
    public:

        typedef typename math::Compound_Channel_Type<PixelT>::type channel_type;

        Channel_Normalize_Functor( channel_type old_min,
                                   channel_type old_max,
                                   channel_type new_min,
                                   channel_type new_max )
            : m_old_min( old_min ),
              m_new_min( new_min )
        {
            if( old_max == old_min )
            {
                m_old_to_new_ratio = 0.0;
            }
            else
            {
                m_old_to_new_ratio = (new_max - new_min)/(double)(old_max - old_min);
            }
        }

        template <typename ChannelT>
        ChannelT operator()( ChannelT value ) const
        {
            return (ChannelT)((value - m_old_min) * m_old_to_new_ratio + m_new_min);
        }

    private:

        channel_type m_old_min, m_new_min;
        double m_old_to_new_ratio;
}; // End of Channel_Normalize_Functor class

/**
 * Normalizes pixel data to specified range, but does not touch alpha 
 * channels
*/
template <typename PixelT>
class Channel_Normalize_Retain_Alpha_Functor: public math::Unary_Return_Same_Type
{
    public:

        typedef typename math::Compound_Channel_Type<PixelT>::type      channel_type;
        
        typedef typename pix::Pixel_Without_Alpha<PixelT>::type   non_alpha_type;
        
        typedef Channel_Normalize_Functor<non_alpha_type>         norm_func_type;

        Channel_Normalize_Retain_Alpha_Functor( channel_type old_min,
                                                channel_type old_max,
                                                channel_type new_min,
                                                channel_type new_max )
            : m_compound_func( norm_func_type( old_min,
                                               old_max,
                                               new_min,
                                               new_max ) )
        {
        }

        PixelT operator()( PixelT value ) const
        {
            if (is_transparent(value))
            {
                return value;
            }
            else
            {
                PixelT result;
                non_alpha_channels(result) = m_compound_func( non_alpha_channels( value ) );
                alpha_channel(result)      = alpha_channel(value);
                return result;
            }
        }

    private:

        cmp::Unary_Compound_Functor<norm_func_type, non_alpha_type> m_compound_func;
}; // End of Channel_Normalize_Retain_Alpha_Functor class


/**
 * Renormalize the values in an image to fall within the range
 * [low,high), but leave the values in the alpha channel untouched.
 */
template <typename ImageT>
Per_Pixel_View_Unary<ImageT, Channel_Normalize_Retain_Alpha_Functor<typename ImageT::pixel_type> >
    normalize_retain_alpha( const Image_Base<ImageT>&                 image,
                            typename pix::Image_Channel_Type<ImageT>::type old_low,
                            typename pix::Image_Channel_Type<ImageT>::type old_high,
                            typename pix::Image_Channel_Type<ImageT>::type new_low,
                            typename pix::Image_Channel_Type<ImageT>::type new_high  )
{
    typedef Channel_Normalize_Retain_Alpha_Functor<typename ImageT::pixel_type> func_type;
    func_type func ( old_low, old_high, new_low, new_high );
    return Per_Pixel_View_Unary<ImageT, func_type >( image.impl(), func );
}

/**
 * Renormalize the values in an image to fall within the range [low,high).
 */
template <typename ImageT>
Per_Pixel_View_Unary<ImageT, cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>,
                                                         typename ImageT::pixel_type> >
    normalize( const Image_Base<ImageT>&                      image,
               typename pix::Image_Channel_Type<ImageT>::type old_low,
               typename pix::Image_Channel_Type<ImageT>::type old_high,
               typename pix::Image_Channel_Type<ImageT>::type new_low,
               typename pix::Image_Channel_Type<ImageT>::type new_high  )
{
    typedef cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>,
                                        typename ImageT::pixel_type> func_type;

    func_type func( Channel_Normalize_Functor<typename ImageT::pixel_type>( old_low,
                                                                            old_high,
                                                                            new_low,
                                                                            new_high ) );
    return Per_Pixel_View_Unary<ImageT, func_type >( image.impl(), func );
}

/**
 * Renormalize the values in an image to fall within the range [low,high).
 */
template <class ImageT>
Per_Pixel_View_Unary<ImageT, cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>, typename ImageT::pixel_type> >
    normalize( const Image_Base<ImageT>&                      image,
               typename pix::Image_Channel_Type<ImageT>::type low,
               typename pix::Image_Channel_Type<ImageT>::type high )
{
    typedef cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>,
                                        typename ImageT::pixel_type> func_type;
    
    typename pix::Image_Channel_Type<ImageT>::type old_min;
    typename pix::Image_Channel_Type<ImageT>::type old_max;
    
    min_max_channel_values( image, old_min, old_max );

    func_type func( Channel_Normalize_Functor<typename ImageT::pixel_type>( old_min,
                                                                            old_max,
                                                                            low,
                                                                            high ) );
    
    return Per_Pixel_View_Unary<ImageT, func_type >( image.impl(), func );
}

/**
 * Renormalize the values in an image to fall within the range [0,high).
 * The low end of the range is actually determined by the ChannelRange 
 * type trait but is generally zero.
 */
template <typename ImageT>
Per_Pixel_View_Unary<ImageT, cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>,
                                                         typename ImageT::pixel_type> >
    normalize( const Image_Base<ImageT>&                      image,
               typename pix::Image_Channel_Type<ImageT>::type high )
{
    typedef cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>,
                                        typename ImageT::pixel_type> func_type;
    
    typedef Channel_Range<typename pix::Image_Channel_Type<ImageT>::type> range_type;
    
    typename pix::Image_Channel_Type<ImageT>::type old_min;
    typename pix::Image_Channel_Type<ImageT>::type old_max;
    
    min_max_channel_values( image, old_min, old_max );

    func_type func( Channel_Normalize_Functor<typename ImageT::pixel_type>( old_min,
                                                                            old_max,
                                                                            range_type::min(),
                                                                            high ) );
    
    return Per_Pixel_View_Unary<ImageT, func_type >( image.impl(), func );
}

/**
 * Renormalize the values in an image to fall within the range [min,max),
 * where min and max are determined by the Channel_Range type trait and are
 * generally equal to 0.0 and 1.0 for floating point types and 0 and the largest
 * positve value for integral types.
 */
template <typename ImageT>
Per_Pixel_View_Unary<ImageT, cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>,
                                                         typename ImageT::pixel_type> >
    normalize( const Image_Base<ImageT>& image )
{
    typedef cmp::Unary_Compound_Functor<Channel_Normalize_Functor<typename ImageT::pixel_type>,
                                        typename ImageT::pixel_type> func_type;

    typedef Channel_Range<typename pix::Image_Channel_Type<ImageT>::type> range_type;
    
    typename pix::Image_Channel_Type<ImageT>::type old_min;
    typename pix::Image_Channel_Type<ImageT>::type old_max;
    
    min_max_channel_values( image, old_min, old_max );
    
    func_type func( Channel_Normalize_Functor<typename ImageT::pixel_type>( old_min,
                                                                            old_max,
                                                                            range_type::min(),
                                                                            range_type::max() ) );
    return Per_Pixel_View_Unary<ImageT, func_type >( image.impl(), func );
}

} // End of tmns::image::ops namespace