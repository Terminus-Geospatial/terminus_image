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
 * @file    pixel_operations.hpp
 * @author  Marvin Smith
 * @date    8/21/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/image/pixel/Pixel_Cast_Utilities.hpp>
#include <terminus/image/pixel/Pixel_Mask.hpp>
#include <terminus/image/types/for_each_pixel.hpp>
#include <terminus/math/types/Math_Functors.hpp>
#include <terminus/math/types/Functors.hpp>

// C++ Libraries
#include <vector>

namespace tmns::image::ops {

/**
 * Element-Wise Min/Max Accumulator
*/
template <typename ValueT>
class EW_Min_Max_Accumulator : public math::Return_Fixed_Type<void>
{
    public:
    
        EW_Min_Max_Accumulator() = default;

        /**
         * Add another element for comparison
        */
        void operator()( const ValueT& arg )
        {
            if ( !m_valid )
            {
                m_min = arg;
                m_max = arg;
                m_valid = true;
            }
            else
            {
                for( size_t i = 0; i < math::Compound_Channel_Count<ValueT>::value; i++ )
                {
                    if( arg[i] < m_min[i] )
                    {
                        m_min[i] = arg[i];
                    }
                    else
                    {
                        if ( arg[i] > m_max[i] )
                        {
                            m_max[i] = arg[i];
                        }
                    }
                }
            }
        }

        /**
         * Check if the accumulator is valid yet
        */
        bool is_valid() const
        {
            return m_valid;
        }

        /**
         * Get the minimum value
        */
        Result<ValueT> minimum() const
        {
            if( m_valid )
            {
                return outcome::ok<ValueT>( m_min );
            }
            else
            {
                return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                                      "Accumulator is uninitialized." );
            }
        }

        /**
         * Get the maximum value
        */
        Result<ValueT> maximum() const
        {
            if( m_valid )
            {
                return outcome::ok<ValueT>( m_max );
            }
            else
            {
                return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                                      "Accumulator is uninitialized." );
            }
        }

    private:

        ValueT m_min, m_max;
        bool m_valid { false };

}; // End of EW_Min_Max_Accumulator class

/**
 * Compute the standard deviation
*/
template <typename ValueT>
class EW_Std_Dev_Accumulator : public math::Return_Fixed_Type<void>
{
    public:

        /**
         * Constructor
        */
        EW_Std_Dev_Accumulator()
        {
            m_sum.fill(0);
            m_sum_2.fill(0);
        }

        /**
         * Add another entry to the accumulator
         */
        void operator()( const ValueT& value )
        {
            m_num_samples += 1;
            for ( size_t i = 0; i < math::Compound_Channel_Count<ValueT>::value; i++ )
            {
                m_sum[i] += value[i];
                m_sum_2[i] += value[i]*value[i];
            }
        }

        /**
         * Compute the standard deviation from the 2 summation values
        */
        Result<ValueT> value() const
        {
            if( m_num_samples <= 0 )
            {
                return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                                      "No samples provided yet to accumulator" );
            }
            
            ValueT result;
            for( size_t i = 0; m_sum.size(); i++ )
            {
                result[i] = sqrt( m_sum_2[i] / m_num_samples - ( m_sum[i] / m_num_samples ) 
                                  * ( m_sum[i] / m_num_samples ) );
            }

            return result;
        }

    private:

        /// Extract the underlying channel-type
        typedef typename pix::Pixel_Channel_Type<ValueT>::type channel_type;

        // Get the sum of pixels for each channel
        std::array<channel_type,math::Compound_Channel_Count<ValueT>::value> m_sum;
        std::array<channel_type,math::Compound_Channel_Count<ValueT>::value> m_sum_2;

        /// Number of samples inserted
        double m_num_samples { 0 };

}; // End of EW_Std_Dev_Accumulator Class

/**
 * Elementwise Median Accumulator
*/
template <typename ValueT>
class EW_Median_Accumulator : public math::Return_Fixed_Type<void>
{
    public:

        /**
         * Default Constructor.
        */
        EW_Median_Accumulator() = default;

        /**
         * Add an entry to the accumulator
        */
        void operator()( const ValueT& value )
        {
            for( int i = 0; i < math::Compound_Channel_Count<ValueT>::value; i++ )
            {
                m_values[i].push_back( value[i] );
            }
        }

        /**
         * Compute the median and return it.
        */
        ValueT value()
        {
            if( m_values.empty() )
            {
                std::stringstream sout;
                sout << "Median_Accumulator: no samples in list.";
                tmns::log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }

            ValueT result;
            for( int i = 0; i < math::Compound_Channel_Count<ValueT>::value; i++ )
            {
                result[i] = destructive_median( m_values[i] );
            }
            return result;
        }

    private:

        typedef std::array<std::vector<typename pix::Pixel_Channel_Type<ValueT>::type>,
                           math::Compound_Channel_Count<ValueT>::value> storage_type;

        storage_type m_values;
}; // End of EW_Median_Accumulator class

/**
 * Wrap the underlying accumulator type to handle pixel masks
*/
template <typename AccumulatorT>
struct Pixel_Accumulator : public AccumulatorT
{
    template <typename ArgumentT>
    void operator()( const ArgumentT& pix )
    {
        if ( is_valid( pix ) )
        {
            AccumulatorT::operator()( remove_mask( pix ) );
        }
    }
};


/**
 * Get the minimum pixel value in the image
*/
template <typename ImageT>
typename std::enable_if_t<math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::value,
                                            typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>
    min_pixel_value( const Image_Base<ImageT>& image )
{
    typedef typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type accum_type;
    Pixel_Accumulator<EW_Min_Max_Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.minimum();
}

/**
 * Get the minimum pixel value in the image (Masked types)
*/
template <typename ImageT>
typename std::enable_if_t<std::negation_v<typename math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::value>,
                                                                     typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>
    min_pixel_value( const Image_Base<ImageT>& image )
{
    return min_channel_value( image );
}

/**
 * Get the maximum pixel value in the image
*/
template <typename ImageT>
typename std::enable_if_t<math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::value,
                          typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>
    max_pixel_value( const Image_Base<ImageT>& image )
{
    typedef typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type accum_type;
    Pixel_Accumulator<EW_Min_Max_Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.maximum();
    return max_channel_value( image );
}

/**
 * Get the maximum pixel in the image (masked)
*/
template <typename ImageT>
typename std::enable_if_t<std::negation_v<typename math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::value>,
                                                                     typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>
    max_pixel_value( const Image_Base<ImageT>& image )
{
    return max_channel_value( image );
}

/**
 * Get the min and max pixel values
*/
template <typename ImageT>
void min_max_pixel_values( const Image_Base<ImageT>& image,
                           typename std::enable_if_t<math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::value>::value, 
                           typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::type &min,
                           typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type        &max )
{
    typedef typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type accum_type;
    Pixel_Accumulator<EW_Min_Max_Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    min = accumulator.minimum();
    max = accumulator.maximum();
}

/**
 * Get the min and max pixel values
*/
template <typename ImageT>
void min_max_pixel_values( const Image_Base<ImageT>& image,
                           typename std::enable_if_t<std::negation_v<typename math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::value>, 
                                                     typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>::type& min,
                           typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type        &max ) {
    min_max_channel_values( image, min, max );
  }

/**
 * Compute the sum of all valid pixels in the image.
 */
template <typename ImageT>
typename pix::Pixel_Channel_Cast<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type,
                                 typename math::Accumulator_Type<typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>::type>::type
    sum_of_pixel_values( const Image_Base<ImageT>& image )
{
    typedef typename pix::Pixel_Channel_Cast<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type,
                                                                          typename math::Accumulator_Type<typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>::type>::type accum_type;
    Pixel_Accumulator<math::Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.value();
}

/**
 * Get the mean pixel value
 * 
 * This is the method for unmasked pxiel values
*/
template <typename ImageT>
typename pix::Pixel_Channel_Cast<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type,double>::type
    mean_pixel_value( const Image_Base<ImageT>& image )
{
    // This will cast our pixel type from whatever into it's unmasked Pixel-Type, with double-precision
    typedef typename pix::Pixel_Channel_Cast<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type,double>::type accum_type;

    // Given that type, build the accumulator
    Pixel_Accumulator<math::Mean_Accumulator<accum_type> > accumulator;

    auto callback = tmns::core::utility::Progress_Callback_Null();
    for_each_pixel( image, accumulator, callback );
    return accumulator.value();
}

/**
 * Get the standard deviation of the pixel values
*/
template <typename ImageT>
typename std::enable_if_t<math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::value,
                                            typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>
    stddev_pixel_value( const Image_Base<ImageT>& image )
{
    typedef typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type accum_type;
    Pixel_Accumulator<EW_Std_Dev_Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.value();
}


/**
 * Get the standard deviation of the pixel values
*/
template <typename ImageT>
typename std::enable_if_t< std::negation_v<math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>>,
                                                                                          double>
    stddev_pixel_value( const Image_Base<ImageT>& image )
{
    return stddev_channel_value( image );
}

/**
 * Get the median pixel value
 */
template <typename ImageT>
typename std::enable_if_t<math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>::value,
                                            typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>
    median_pixel_value( const Image_Base<ImageT>& image )
{
    typedef typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type accum_type;
    Pixel_Accumulator<EW_Median_Accumulator<accum_type> > accumulator;
    for_each_pixel( image, accumulator );
    return accumulator.value();
}

/**
 * Get the median pixel value
 */
template <typename ImageT>
typename std::enable_if_t<std::negation_v<math::Is_Compound<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type>>,
                                                            typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>
    median_pixel_value( const Image_Base<ImageT>& image )
{
    return median_channel_value( image );
}

/**
 * Computes the weighted mean of the values of all the pixels of an
 * image, using the alpha channel as a weight and assuming
 * pre-multiplied channel values.  This function throws an
 * ArgumentErr() exception if the image has zero size or is
 * completely transparent.  For images with no alpha or mask
 * channel this function is identical to mean_pixel_value().
 */
template <typename ImageT>
typename pix::Pixel_Without_Alpha<typename pix::Pixel_Channel_Cast<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type,
                                                                                                double>::type>::type
    weighted_mean_pixel_value( const Image_Base<ImageT>& image )
{
    typedef typename pix::Pixel_Channel_Cast<typename Unmasked_Pixel_Type<typename ImageT::pixel_type>::type,double>::type accum_type;
    accum_type mean = mean_pixel_value( image );
    if ( pix::Pixel_Has_Alpha<typename ImageT::pixel_type>::value )
    {
        double weight = alpha_channel( mean ) / Channel_Range<typename pix::Pixel_Channel_Type<typename ImageT::pixel_type>::type>::max();
        if( weight == 0 )
        {
            std::stringstream sout;
            sout << "weighted_mean_pixel_value(): no weighted samples";
            tmns::log::error( sout.str() );
            throw std::runtime_error( sout.str() );            
        }
        mean /= weight;
    }
    return non_alpha_channels( mean );
}


/**
 * Computes the weighted mean of the values of the channels of all
 * of the valid pixels of an image (excluding alpha or mask
 * channels), using the alpha channel as a weight.  This function
 * throws an ArgumentErr() exception if the image has zero size or
 * is completely transparent.  For images with no alpha channel
 * this function is identical to mean_channel_value().
 */
template <typename ImageT>
double weighted_mean_channel_value( const Image_Base<ImageT>& image )
{
    return mean_channel_value( weighted_mean_pixel_value( image ) );
}


} // End of tmns::image::ops namespace