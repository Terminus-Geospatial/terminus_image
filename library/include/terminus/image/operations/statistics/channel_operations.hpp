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
 * @file    statistics.hpp
 * @author  Marvin Smith
 * @date    8/21/2023
*/
#pragma once

// Terminus Image Libraries
#include <terminus/image/pixel/Channel_Cast_Utilities.hpp>
#include <terminus/image/pixel/Pixel_Cast_Utilities.hpp>
#include <terminus/image/types/for_each_pixel.hpp>
#include <terminus/math/types/Math_Functors.hpp>

namespace tmns {
namespace image::ops {
/**
 * An adapter to help applying an accumulator to all channels of
 * all valid pixels in an image.
 */
template <typename AccumulatorT>
class Channel_Accumulator : public AccumulatorT
{
    public:
    
        template <typename ArgumentT>
        void operator()( const ArgumentT& pix )
        {
            if ( is_valid(pix) )
            {
                pix::compound_apply_in_place( (AccumulatorT&)*this,
                                              remove_mask( pix ) );
            }
        }
}; // End of Channel_Accumulator class

/**
 * Locate the minimum value of of all channels stored in all planes of the 
 * image
*/
template <class ImageT>
typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type
    min_channel_value( const Image_Base<ImageT>& image )
{
    typedef typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type accum_type;
    Channel_Accumulator<math::Min_Max_Accumulator<accum_type> > accumulator;
    core::utility::Progress_Callback_Null junk_callback;
    for_each_pixel( image, accumulator, junk_callback );
    return accumulator.minimum();
}

/**
 * Compute the maximum value stored in all of the channels of all of the planes of the images.
 */
template <typename ImageT>
typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type
    max_channel_value( const Image_Base<ImageT>& image )
{
    typedef typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type accum_type;
    Channel_Accumulator<math::Min_Max_Accumulator<accum_type> > accumulator;
    core::utility::Progress_Callback_Null junk_callback;
    for_each_pixel( image, accumulator, junk_callback );
    return accumulator.maximum();
}

/**
 * Simultaneously compute the min and max value in all of the channels
 * of all of the planes of the image.
 */
template <typename ImageT>
void min_max_channel_values( const Image_Base<ImageT>&                                            image,
                             typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type& min,
                             typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type& max )
{
    typedef typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type accum_type;
    Channel_Accumulator<math::Min_Max_Accumulator<accum_type> > accumulator;
    core::utility::Progress_Callback_Null junk_callback;
    for_each_pixel( image, accumulator, junk_callback );
    min = accumulator.minimum();
    max = accumulator.maximum();
}

} // End of image::ops namespace

namespace math {
/**
 * Compute the sum of all the channels of all the valid pixels of the image.
 */
template <typename ImageT>
typename Accumulator_Type<typename image::pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>::type
    sum_of_channel_values( const image::Image_Base<ImageT>& image )
{
    typedef typename math::Accumulator_Type<typename image::pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>::type accum_type;
    image::ops::Channel_Accumulator<::tmns::math::Accumulator<accum_type> > accumulator;
    core::utility::Progress_Callback_Null junk_callback;
    for_each_pixel( image, accumulator, junk_callback );
    return accumulator.value();
}
} // End of math namespace

namespace image::ops {

/**
 * Computes the mean of the values of the channels of all of the
 * valid (non-masked) pixels of an image (including alpha but
 * excluding mask channels).
 */
template <typename ImageT>
double mean_channel_value( const Image_Base<ImageT>& image )
{
    typedef typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type accum_type;
    Channel_Accumulator<math::Mean_Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.value();
}

/**
 * Computes the standard deviation of the values of all the channels of all of
 * the planes of an image.  For images that have an alpha channel, this function
 * computes the weighted standard deviation, using the alpha channel as a weight
 * and assuming pre-multiplied pixel values.  This function throws an ArgumentErr()
 * exception if the image has zero size or is completely transparent.
 * 
 * Note: This function computes the total stanadard deviation, not the sample standard
 * deviation as was computed by previous versions.  If you need the sample standard
 * deviation, just multiply the result by sqrt(num_samples/(num_samples-1)), where
 * num_samples=channels*cols*rows*planes.  Note that the concept of sample standard
 * deviation is not particularly meaningful for images with alpha channels.
 */
template <typename ImageT>
double stddev_channel_value( const Image_Base<ImageT>& image )
{
    typedef typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type channel_type;
    Channel_Accumulator<math::Std_Dev_Accumulator<channel_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.value();
}

/**
 * Computes the median channel value of an image.  Only non-alpha channels of
 * valid (e.g.  non-transparent) pixels are considered. This function computes
 * the median by sorting all the channel values in the image, which is time and memory
 * intensive, so this operation is not recommended for large images.
 */
template <typename ImageT>
typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type
    median_channel_value( const Image_Base<ImageT>& image )
{
    typedef typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type accum_type;
    Channel_Accumulator<math::Median_Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.value();
}

} // End of image::ops namespace
} // End of tmns namespace