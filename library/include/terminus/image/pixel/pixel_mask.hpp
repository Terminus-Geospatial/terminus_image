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
 * @file    Pixel_Mask.hpp
 * @author  Marvin Smith
 * @date    8/21/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/image/pixel/Channel_Range.hpp>
#include <terminus/math/types/Compound_Types.hpp>
#include <terminus/math/types/Fundamental_Types.hpp>

namespace tmns {
namespace image {
/**
 * Wraps any pixel-type with a "valid" channel.  Math operations applied to 
 * invalid pixels will themselves return invalid.
*/
template <typename PixelT>
class Pixel_Mask : public Pixel_Base<Pixel_Mask<PixelT> >
{
    public:

        /// Underlying Channel-Type
        typedef typename math::Compound_Channel_Type<PixelT>::type channel_type;

        /**
         * Default Constructor (Invalid by default)
         */
        Pixel_Mask() = default;

        /**
         * Conversion from different Pixel_Mask Type
        */
        template <typename OtherPixelT>
        Pixel_Mask( Pixel_Mask<OtherPixelT> other )
        {
            // The child type's conversions will do there work here
            m_child = PixelT( other.child() );

            if( other.valid() )
            {
                m_valid = Channel_Range<channel_type>::max();
            }
            else
            {
                m_valid = Channel_Range<channel_type>::min();
            }
        }

        /**
         * Implicit conversion from raw channel value.  This will default
         * to valid, since there is a legitimate value to start with.
        */
        template <typename ValueT>
        Pixel_Mask( const ValueT& pix )
        {
            m_child = PixelT( pix );
            m_valid = Channel_Range<channel_type>::max();
        }

        /**
         * Construct a pixel with the given channel values
         */
        Pixel_Mask( const channel_type& a0,
                    const channel_type& a1 )
        {
            m_child[0] = a0;
            m_child[1] = a1;
            m_valid = Channel_Range<channel_type>::max();
        }

        /**
         * Construct a pixel with the given channel values
         */
        Pixel_Mask( const channel_type& a0,
                    const channel_type& a1,
                    const channel_type& a2 )
        {
            m_child[0] = a0;
            m_child[1] = a1;
            m_child[2] = a2;
            m_valid = Channel_Range<channel_type>::max();
        }

        /**
         * Construct a pixel with the given channel values
         */
        Pixel_Mask( const channel_type& a0,
                    const channel_type& a1,
                    const channel_type& a2,
                    const channel_type& a3 )
        {
            m_child[0] = a0;
            m_child[1] = a1;
            m_child[2] = a2;
            m_child[3] = a3;
            m_valid = Channel_Range<channel_type>::max();
        }

        /**
         * Returns the status of the valid flag
        */
        channel_type valid() const
        {
            return m_valid;
        }

        /**
         * Force pixel to be invalid
         */
        void invalidate()
        {
            m_valid = Channel_Range<channel_type>::min();
        }

        /**
         * Force pixel to be valid
         */
        void validate()
        {
            m_valid = Channel_Range<channel_type>::max();
        }

        /**
         * Toggle the valid of the valid flag
         */
        void toggle() 
        {
            if( m_valid )
            {
                invalidate();
            }
            else
            {
                validate();
            }
        }

        /**
         * Return the underlying pixel-type
         */
        PixelT&  child()
        {
            return m_child;
        }

        /**
         * Return a const reference to the underlying pixel type
        */
        PixelT const& child() const
        {
            return m_child;
        }

        /**
         * Automatic down-cast to the raw-channel value in numeric
         * contexts.  This should fail whenever the number of channels
         * (excluding the mask) exceeds one.
         */
        operator channel_type() const
        {
            static_assert( math::Compound_Channel_Count<PixelT>::value == 1 );
            return compound_select_channel<const channel_type&>( m_child, 0 );
        }

        /**
         * Get a specific channel
         * Note:  1 past the last channel is the valid flag
         *       i.e.:  4 for RGB will return valid.
         */
        channel_type& operator[] ( size_t i )
        {
            if( i == math::Compound_Channel_Count<PixelT>::value )
            {
                return m_valid;
            }
            else
            {
                compound_select_channel<const channel_type&>( m_child, i );
            }
        }

        /**
         * Get a specific channel
         * Note:  1 past the last channel is the valid flag
         *       i.e.:  4 for RGB will return valid.
         */
        channel_type const& operator[] ( size_t i ) const
        {
            if( i == math::Compound_Channel_Count<PixelT>::value )
            {
                return m_valid;
            }
            else
            {
                compound_select_channel<const channel_type&>( m_child, i );
            }
        }
   
        /**
         * Get a specific channel
         * Note:  1 past the last channel is the valid flag
         *       i.e.:  4 for RGB will return valid.
         */
        channel_type& operator() ( size_t i )
        {
            if( i == math::Compound_Channel_Count<PixelT>::value )
            {
                return m_valid;
            }
            else
            {
                compound_select_channel<const channel_type&>( m_child, i );
            }
        }

        /**
         * Get a specific channel
         * Note:  1 past the last channel is the valid flag
         *       i.e.:  4 for RGB will return valid.
         */
        channel_type const& operator() ( size_t i ) const
        {
            if( i == math::Compound_Channel_Count<PixelT>::value )
            {
                return m_valid;
            }
            else
            {
                compound_select_channel<const channel_type&>( m_child, i );
            }
        }

    private:
    
        // Child Pixel Data
        PixelT m_child;
        
        /// Validity Flag
        channel_type m_valid { Channel_Range<channel_type>::min() };

}; // End of Pixel_Mask Class

/****************************************/
/*          Pixel Type Traits           */
/****************************************/

/**
 * Check if the pixel is transparent
*/
template <typename PixelT>
bool is_transparent( const Pixel_Mask<PixelT>& pixel )
{
    return !pixel.valid();
}

/**
 * Check if pixel is opaque
*/
template <typename PixelT>
bool is_opaque( const Pixel_Mask<PixelT>& pixel )
{
    return pixel.valid();
}

/**
 * Check validity (non-masked pixel)
*/
template <typename PixelT>
bool is_valid( const PixelT& )
{
    return true;
}

/**
 * Overload for the Pixel-Mask type
*/
template <typename PixelT>
bool is_valid( const Pixel_Mask<PixelT>& pixel )
{
    return pixel.valid();
}

/**
 * Generic no-op to "validate" the pixel
*/
template <typename PixelT>
void validate( PixelT& )
{
    return;
}

/**
 * Validation method for Pixel-Mask types.
*/
template <typename PixelT>
void validate( const Pixel_Mask<PixelT>& pixel )
{
    pixel.validate();
}

/**
 * Generic no-op to "invalidate" the pixel
*/
template <typename PixelT>
void invalidate( PixelT& )
{
    return;
}

/**
 * Invalidation method for Pixel-Mask Types
*/
template <typename PixelT>
void invalidate( Pixel_Mask<PixelT>& pixel )
{
    pixel.invalidate();
}

/**
 * Generic no-op to "toggle" the pixel
*/
template <typename PixelT>
void toggle( PixelT& )
{
    return;
}

/**
 * Toggle method for Pixel_Mask types
*/
template <typename PixelT>
void toggle( Pixel_Mask<PixelT>& pixel )
{
    pixel.toggle();
}

/**
 * Strip the pixel mask.  For non Pixel_Mask types, 
 * this simply returns itself.
*/
template <typename PixelT>
PixelT& remove_mask( PixelT& pixel )
{
    return pixel;
}

/**
 * Strip the pixel mask.  For non Pixel_Mask types, 
 * this simply returns itself.
*/
template <typename PixelT>
const PixelT& remove_mask( const PixelT& pixel )
{
    return pixel;
}

/**
 * Strip the pixel-mask.  This returns the child reference
*/
template <typename PixelT>
PixelT& remove_mask( Pixel_Mask<PixelT>& pixel )
{
    return pixel.child();
}

/**
 * Strip the pixel mask (const types)
*/
template <typename PixelT>
const PixelT& remove_mask( const Pixel_Mask<PixelT>& pixel )
{
    return pixel.child();
}

/**
 * Print to output stream
*/
template <typename PixelT>
std::ostream& operator << ( std::ostream&             os,
                            const Pixel_Mask<PixelT>& pix )
{
    return os << "Pixel_Mask( " << pix.child() << " : " << numeric(pix.valid()) << " )";
}

} // End of namespace image

namespace math {

/************************************************/
/*          Standard Compound Traits            */
/************************************************/

/**
 * Get the underlying Channel-Type
*/
template <typename PixelT>
struct Compound_Channel_Type<image::Pixel_Mask<PixelT> >
{
    typedef typename Compound_Channel_Type<PixelT>::type type;
};

/**
 * Get the number of channels
*/
template <typename PixelT>
struct Compound_Channel_Count<image::Pixel_Mask<PixelT> >
{
    static const size_t value = Compound_Channel_Count<PixelT>::value + 1;
};

/**
 * Cast the channel-type
*/
template <typename OldPixelT,
          typename NewChannelT>
struct Compound_Channel_Cast<image::Pixel_Mask<OldPixelT>, NewChannelT>
{
    typedef image::Pixel_Mask<typename Compound_Channel_Cast<OldPixelT,NewChannelT>::type> type;
};

/**
 * Cast the channel-type (const types)
*/
template <typename OldPixelT,
          typename NewChannelT>
struct Compound_Channel_Cast<image::Pixel_Mask<OldPixelT>, const NewChannelT>
{
    typedef image::Pixel_Mask<typename Compound_Channel_Cast<OldPixelT,NewChannelT>::type> type;
};

} // End of namespace math

namespace image {

/**
 * Mark pixel as being masked
*/
template <typename PixelT>
struct Is_Masked : public std::false_type{};

template <typename PixelT>
struct Is_Masked<Pixel_Mask<PixelT>> : public std::true_type{};

/**
 * Compute the mean channel value of a compound Pixel_Mask type.
*/
template <typename PixelT>
std::enable_if_t<math::Is_Scalar_Or_Compound<PixelT>::value,double>
    mean_channel_value( const Pixel_Mask<PixelT>& arg )
{
    typedef typename math::Compound_Channel_Type<PixelT>::type channel_type;
    if( arg.valid() )
    {
        size_t num_channels = math::Compound_Channel_Count<PixelT>::value;
        double accum = 0;
        for( size_t i=0; i<num_channels-1; ++i )
        {
            accum += compound_select_channel<channel_type const&>( arg, i );
        }
        return accum / num_channels;
    } 
    else
    {
        return 0;
    }
}




  /// These are handy for determining what the masked type is for a
  /// given pixel type.
  template <class PixelT>
  struct Masked_Pixel_Type { typedef Pixel_Mask<PixelT> type; };

/**
 * Most importantly, we want to make sure that we only wrap each
 * pixel in one layer of Pixel_Mask<>.
 */
template <typename ChildT>
struct Masked_Pixel_Type<Pixel_Mask<ChildT> > { typedef Pixel_Mask<ChildT> type; };

/**
 * These are handy for determining what the masked type is for a
 * given pixel type.
 */
template <typename PixelT>
struct Unmasked_Pixel_Type { typedef PixelT type; };

/**
 * Most importantly, we want to make sure that we only wrap each
 * pixel in one layer of Pixel_Mask<>.
 */
template <typename ChildT>
struct Unmasked_Pixel_Type<Pixel_Mask<ChildT> > { typedef ChildT type; };


/**
 * Wrapper functor to call a child functor only on valid input pixels.
 *  - Use this to augment an existing functor with Pixel_Mask functionality.
 */
template <typename F,
          typename PixelT>
class Functor_Mask_Wrapper
{
    public:

        /**
         * Constructor, makes a copy of the input functor.
         */
        Functor_Mask_Wrapper( const F& functor ) : m_functor(functor){}

        /**
         * Call the child functor only if the input pixel is valid
         */
        void operator()( const PixelT& pixel )
        {
            if( is_valid( pixel ) )
            {
                m_functor(pixel.child()); // Call functor on non-masked pixel
            }
        }

        /**
         * Grant access to the child so that the user can retrieve the results.
         */
        F const& child() const { return m_functor; }

    private:

        F m_functor;
}; // End class Functor_Mask_Wrapper

namespace cmp {

/**
 * Applies a functor individually to each channel or a pair of input pixels.
 * It will set the result as invalid if the input masked pixel is invalid
*/
template <typename FunctorT,
          typename ChildPixel1T,
          typename ChildPixel2T>
class Binary_Compound_Functor<FunctorT,
                              Pixel_Mask<ChildPixel1T>,
                              Pixel_Mask<ChildPixel2T> >
{
    public:

        /**
         * Constructor
        */
        Binary_Compound_Functor() : func() {}
        
        /**
         * Constructor
        */
        Binary_Compound_Functor( const FunctorT& func ) : func(func) {}

        /**
         * Result Type
        */
        template <typename ArgsT>
        struct result {};

        template <typename F,
                  typename Argument1T,
                  typename Argument2T>
        struct result<F(Argument1T,Argument2T)>
        {
            typedef typename math::Compound_Channel_Type<Argument1T>::type arg1_type;
            typedef typename math::Compound_Channel_Type<Argument2T>::type arg2_type;
            typedef typename std::invoke_result<FunctorT,arg1_type,arg2_type>::type result_type;
            typedef typename math::Compound_Channel_Cast<Argument1T,result_type>::type type;
        };

        /**
         * Run the function operator
         */
        template <typename Argument1T,
                  typename Argument2T>
        typename result<Binary_Compound_Functor(Argument1T,Argument2T)>::type
            operator()( const Argument1T& arg1, 
                        const Argument2T& arg2 ) const
        {
            typedef typename result<Binary_Compound_Functor(Argument1T,Argument2T)>::type result_type;
            return Helper<math::Is_Compound<result_type>::value,
                          math::Compound_Channel_Count<result_type>::value,
                          result_type,
                          Argument1T,
                          Argument2T>::construct(func,arg1,arg2);
        }


    private:

        // The general multi-channel case
        template <bool     CompoundB,
                  size_t   ChannelsN,
                  typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper
        {
            static ResultT construct( const FunctorT&   func,
                                      const Argument1T& arg1,
                                      const Argument2T& arg2 )
            {
                ResultT result;
                for( size_t i = 0; i < ChannelsN - 1; ++i )
                {
                    result[i] = func( arg1[i], arg2[i] );
                }
                if( arg1.valid() && arg2.valid() )
                {
                    result.validate();
                }
                return result;
            }
        };

        // Specialization for one-channel + 1 "valid pixel" channel
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,2,ResultT,Argument1T,Argument2T>
        {
            static inline ResultT construct( const FunctorT&   func,
                                             const Argument1T& arg1,
                                             const Argument2T& arg2 )
            {
                ResultT result( func( arg1[0], arg2[0] ) );
                if( !arg1.valid() || !arg2.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        }; // End of Helper Struct

        // Specialization for two-channel + 1 valid pixel channel types
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,3,ResultT,Argument1T,Argument2T>
        {
            static ResultT construct( const FunctorT&   func,
                                      const Argument1T& arg1,
                                      const Argument2T& arg2 )
            {
                ResultT result( func(arg1[0],arg2[0]), func(arg1[1],arg2[1]) );
                if( !arg1.valid() || !arg2.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        }; // End of Helper Struct

        // Specialization for three-channel + 1 valid pixel channel types
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,4,ResultT,Argument1T,Argument2T>
        {
            static ResultT construct( const FunctorT&   func,
                                      const Argument1T& arg1,
                                      const Argument2T& arg2 )
            {
                ResultT result( func( arg1[0], arg2[0] ),
                                func( arg1[1], arg2[1] ),
                                func( arg1[2], arg2[2] ) );
                if( !arg1.valid() || !arg2.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        }; // End of Helper Struct

        // Specialization for four-channel + 1 valid pixel channel types
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,5,ResultT,Argument1T,Argument2T>
        {
            static ResultT construct( const FunctorT&   func, 
                                      const Argument1T& arg1,
                                      const Argument2T& arg2 )
            {
                ResultT result( func( arg1[0], arg2[0] ),
                                func( arg1[1], arg2[1] ),
                                func( arg1[2], arg2[2] ),
                                func( arg1[3], arg2[3] ) );
                if( !arg1.valid() || !arg2.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        }; // End of Helper Struct

        // Internal Functor
        FunctorT func;

}; // End of Binary_Compound_Functor Class

/**
 * Applies a functor individually to each channel of a pair of input masked pixels
 * in-place, retaining the original validity of the input pixel.
 */
template <typename FunctorT,
          typename ChildPixel1T,
          typename ChildPixel2T>
class Binary_In_Place_Compound_Functor<FunctorT, 
                                       Pixel_Mask<ChildPixel1T>,
                                       Pixel_Mask<ChildPixel2T> >
{
    public:
    
        /**
         * Default Constructor
        */
        Binary_In_Place_Compound_Functor() = default;
        
        Binary_In_Place_Compound_Functor( const FunctorT& func ) : func(func) {}

        template <typename ArgsT>
        struct result {};

        template <typename F,
                  typename Argument1T,
                  typename Argument2T>
        struct result<F(Argument1T,Argument2T)>
        {
            typedef Argument1T& type;
        };

        template <typename Argument1T,
                  typename Argument2T>
        typename result<Binary_In_Place_Compound_Functor(Argument1T,Argument2T)>::type
            operator()( Argument1T&       arg1, 
                        const Argument2T& arg2 ) const
        {
            return Helper<math::Is_Compound<Argument1T>::value,
                          math::Compound_Channel_Count<Argument1T>::value,
                          Argument1T,
                          Argument2T>::apply(func,arg1,arg2);
        }

    private:

        // The general multi-channel case
        template <bool     CompoundB,
                  size_t   ChannelsN,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper
        {
            static Argument1T& apply( const FunctorT&   func,
                                      Argument1T&       arg1,
                                      const Argument2T& arg2 )
            {
                for( size_t i = 0; i < ChannelsN - 1; ++i )
                {
                    func(arg1[i],arg2[i]);
                }
                if( !arg2.valid() )
                {
                    arg1.invalidate();
                }
                return arg1;
            }
        };

        // Specialization for one-channel types + 1 "valid pixel" channel
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,2,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&   func,
                                      Argument1T&       arg1,
                                      const Argument2T& arg2 )
            {
                func( arg1[0], arg2[0] );
                if( !arg2.valid() )
                {
                    arg1.invalidate();
                }
                return arg1;
            }
        };

        // Specialization for two-channel types + 1 "valid pixel" channel
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,3,Argument1T,Argument2T>
        {
            static inline Argument1T& apply( const FunctorT&   func,
                                             Argument1T&       arg1,
                                             const Argument2T& arg2 )
            {
                func( arg1[0], arg2[0] );
                func( arg1[1], arg2[1] );
                if( !arg2.valid() )
                {
                    arg1.invalidate();
                }
                return arg1;
            }
        };

        // Specialization for three-channel types + 1 "valid pixel" channel
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,4,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&   func,
                                      Argument1T&       arg1,
                                      const Argument2T& arg2 )
            {
                func( arg1[0], arg2[0] );
                func( arg1[1], arg2[1] );
                func( arg1[2], arg2[2] );
                if( !arg2.valid() )
                {
                    arg1.invalidate();
                }
                return arg1;
            }
        };

        // Specialization for four-channel types + 1 "valid pixel" channel
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,5,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&   func,
                                      Argument1T&       arg1,
                                      const Argument2T& arg2 )
            {
                func( arg1[0], arg2[0] );
                func( arg1[1], arg2[1] );
                func( arg1[2], arg2[2] );
                func( arg1[3], arg2[3] );
                if( !arg2.valid() )
                {
                    arg1.invalidate();
                }
                return arg1;
            }
        };

        FunctorT func;

}; // End of Binary_In_Place_Compound_Functor class


/**
 * Applies a functor individually to each channel of an input masked pixel,
 * and sets the result as invalid if the input pixel was invalid.
 */
template <typename FunctorT,
          typename ChildPixelT>
class Unary_Compound_Functor<FunctorT, Pixel_Mask<ChildPixelT> >
{
    public:
        Unary_Compound_Functor() = default;

        Unary_Compound_Functor( const FunctorT& func ) : func(func) {}

        template <class ArgsT>
        struct result {};

        /**
         * Return Type
        */
        template <typename F,
                  typename ArgumentT>
        struct result<F(ArgumentT)>
        {
            typedef typename math::Compound_Channel_Type<ArgumentT>::type             arg_type;
            typedef typename std::invoke_result<FunctorT(arg_type)>::type       result_type;
            typedef typename math::Compound_Channel_Cast<ArgumentT,result_type>::type type;
        };

        /**
         * Function operator
        */
        template <typename ArgumentT>
        typename result<Unary_Compound_Functor(ArgumentT)>::type
            operator()( const ArgumentT& arg ) const
        {
            typedef typename result<Unary_Compound_Functor(ArgumentT)>::type result_type;
            return Helper<math::Is_Compound<result_type>::value,
                          math::Compound_Channel_Count<result_type>::value,
                          result_type,
                          ArgumentT>::construct(func,arg);
        }

    private:

        // The general multi-channel case
        template <bool     CompoundB,
                  size_t   ChannelsN,
                  typename ResultT,
                  typename ArgumentT>
        struct Helper
        {
            static ResultT construct( const FunctorT&  func,
                                      const ArgumentT& arg )
            {
                ResultT result;
                for( size_t i = 0; i < ChannelsN - 1; ++i )
                {
                    result[i] = func( arg[i] );
                }
                if( arg.valid() )
                {
                    result.validate();
                }
                return result;
            }
        };

        // Specialization for one-channel + 1 "valid pixel" channel
        template <typename ResultT,
                  typename ArgumentT>
        struct Helper<true,2,ResultT,ArgumentT>
        {
            static ResultT construct( const FunctorT&   func,
                                      const ArgumentT&  arg )
            {
                ResultT result( func(arg[0]) );
                if( !arg.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        };

        // Specialization for two-channel + 1 valid pixel channel types
        template <typename ResultT,
                  typename ArgumentT>
        struct Helper<true,3,ResultT,ArgumentT>
        {
            static ResultT construct( const FunctorT&  func,
                                      const ArgumentT& arg )
            {
                ResultT result( func( arg[0] ),
                                func( arg[1] ) );
                if( !arg.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        };

        // Specialization for three-channel + 1 valid pixel channel types
        template <typename ResultT,
                  typename ArgumentT>
        struct Helper<true,4,ResultT,ArgumentT>
        {
            static ResultT construct( const FunctorT&  func,
                                      const ArgumentT& arg )
            {
                ResultT result( func( arg[0] ),
                                func( arg[1] ),
                                func( arg[2] ) );
                if( !arg.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        };

        // Specialization for four-channel + 1 valid pixel channel types
        template <typename ResultT,
                  typename ArgumentT>
        struct Helper<true,5,ResultT,ArgumentT>
        {
            static ResultT construct( const FunctorT&  func,
                                      const ArgumentT& arg )
            {
                ResultT result( func( arg[0] ),
                                func( arg[1] ),
                                func( arg[2] ),
                                func( arg[3] ) );
                if( !arg.valid() )
                {
                    result.invalidate();
                }
                return result;
            }
        };

        FunctorT func;
};

// *******************************************************************
// Unary in-place elementwise compound type functor.
// *******************************************************************


/**
 * Applies a functor individually to each channel of an input masked pixel in-place,
 * retaining the original validity of the input pixel.
 */
template <typename FunctorT,
          typename ChildPixelT>
class Unary_In_Place_Compound_Functor<FunctorT, Pixel_Mask<ChildPixelT> >
{
    public:

        typedef typename std::add_lvalue_reference<FunctorT>::type func_ref;
    
        /**
         * Default Constructor
         */
        Unary_In_Place_Compound_Functor() = default;

        /**
         * Parameterized Constructor
        */
        Unary_In_Place_Compound_Functor( func_ref func ) : func(func) {}

        /**
         * Result Type
        */
        template <typename ArgsT> struct result {};

        
        template <typename F,
                  typename ArgumentT>
        struct result<F(ArgumentT)>
        {
            typedef ArgumentT& type;
        };

        /**
         * Function Operator
         */
        template <typename ArgumentT>
        ArgumentT& operator()( ArgumentT& arg ) const
        {
            return Helper<math::Is_Compound<ArgumentT>::value,
                          math::Compound_Channel_Count<ArgumentT>::value,ArgumentT>::apply(func,arg);
        }

        /**
         * Function Operator
         */
        template <typename ArgumentT>
        const ArgumentT& operator()( const ArgumentT& arg ) const
        {
            return Helper<math::Is_Compound<ArgumentT>::value,
                          math::Compound_Channel_Count<ArgumentT>::value,
                          const ArgumentT>::apply(func,arg);
        }

    private:

        // The general multi-channel case
        template <bool CompoundB, size_t ChannelsN, typename ArgumentT>
        struct Helper
        {
            static ArgumentT& apply( func_ref func, ArgumentT& arg )
            {
                for( size_t i=0; i<ChannelsN-1; ++i ) func(arg[i]);
                return arg;
            }
        };

        // Specialization for one-channel types + 1 "valid pixel" channel
        template <typename ArgumentT>
        struct Helper<true,2,ArgumentT>
        {
            static ArgumentT& apply( func_ref func, ArgumentT& arg )
            {
                func(arg[0]);
                return arg;
            }
        };

    // Specialization for two-channel types + 1 "valid pixel" channel
    template <class ArgT>
    struct Helper<true,3,ArgT> {
      static inline ArgT& apply( func_ref func, ArgT& arg ) {
        func(arg[0]);
        func(arg[1]);
        return arg;
      }
    };

    // Specialization for three-channel types + 1 "valid pixel" channel
    template <class ArgT>
    struct Helper<true,4,ArgT> {
      static inline ArgT& apply( func_ref func, ArgT& arg ) {
        func(arg[0]);
        func(arg[1]);
        func(arg[2]);
        return arg;
      }
    };

        /**
         * Specialization for four-channel types + 1 "valid pixel" channel
         */
        template <typename ArgumentT>
        struct Helper<true,5,ArgumentT>
        {
            static inline ArgumentT& apply( func_ref   func,
                                            ArgumentT& arg )
            {
                func( arg[0] );
                func( arg[1] );
                func( arg[2] );
                func( arg[3] );
                return arg;
            }
        };

        FunctorT func;

}; // End of Unary_In_Place_Compound_Functor class

} // End of cmp namespace

/**
 * Special Quotient Safe Functors for Pixel Mask types
 * result = arg1 / arg2, if arg2==0 then result = 0.
 */
struct Arg_Arg_Masked_Safe_Quotient_Functor : math::Binary_Return_Template_Type<math::Quotient_Type>
{
    template <typename Argument1T,
              typename Argument2T>
    typename math::Quotient_Type<Pixel_Mask<Argument1T>, Pixel_Mask<Argument2T> >::type
        operator()( const Pixel_Mask<Argument1T>& arg1,
                    const Pixel_Mask<Argument2T>& arg2 ) const
    {
        if( arg2.child() == Argument2T() ) // If arg2 == 0
        { 
            if ( is_valid(arg1) && is_valid( arg2 ) ) // If both inputs are valid
            {
                typename math::Quotient_Type<Pixel_Mask<Argument1T>,
                                             Pixel_Mask<Argument2T> >::type temp;
                temp.validate();
                return temp; // Valid
            }
            else
            {
                return typename math::Quotient_Type<Pixel_Mask<Argument1T>,
                                                    Pixel_Mask<Argument2T> >::type(); // Invalid
            }
        }
        else
        {
            return ( arg1 / arg2 );
        }
    }
}; // End of Arg_Arg_Masked_Safe_Quotient_Functor struct

/// result = val / arg, if arg==0 then result = 0.
template <typename ValueT>
struct Val_Arg_Masked_Safe_Quotient_Functor : math::Unary_Return_Binary_Template_Bind_1st<math::Quotient_Type,ValueT>
{
    public:

      /**
       * Constructor
       */
      Val_Arg_Masked_Safe_Quotient_Functor( const ValueT& value ) : m_value( value ){}

      template <typename ArgumentT>
      typename math::Quotient_Type<ValueT, Pixel_Mask<ArgumentT> >::type
          operator()( const Pixel_Mask<ArgumentT>& arg ) const
      {
          if( arg.child() == ArgumentT() )
          {
              // If both inputs are valid
              if( is_valid( arg ) && is_valid( m_value ) )
              { 
                  typename math::Quotient_Type<ValueT,Pixel_Mask<ArgumentT> >::type temp;
                  validate( temp );
                  return temp;
              }
              else
              {
                  //Invalid
                  return typename math::Quotient_Type<ValueT,Pixel_Mask<ArgumentT> >::type();
              }
          }
          else
          {
              return ( m_value / arg );
          }
      }
  
    private:

        const ValueT m_value;
};

/// arg1 = arg1 / arg2, if arg2==0 then arg1 = 0.
struct Arg_Arg_In_Place_Masked_Safe_Quotient_Functor : math::Binary_Return_1st_Type
{
    template <typename Argument1T,
              typename Argument2T>
    Pixel_Mask<Argument1T>& operator()( const Pixel_Mask<Argument1T>& arg1,
                                        const Pixel_Mask<Argument2T>& arg2 ) const
    {
        if ( arg2.child() == Argument2T() )
        {
            if( is_valid( arg1 ) && 
                is_valid( arg2 ) ) // Inputs are both valid
            {
                // Return zero -> Need to pass in a zero pixel, the default Pixel_Mask constructor sets invalid
                return arg1 = Pixel_Mask<Argument1T>( Argument1T() );
            }
            else
            {
                return arg1 = Pixel_Mask<Argument1T>(); // Inputs are invalid, return invalid
            }
        }
        else
        {
            return arg1= ( Pixel_Mask<Argument1T> )( arg1 / arg2 );
        }
      }
}; // End of Arg_Arg_In_Place_Masked_Safe_Quotient_Functor class


/**
 * Argument Value Addition
 */
template <typename PixelT,
          typename ScalarT>
    typename std::enable_if_t<std::conjunction_v<std::conjunction<typename math::Is_Scalar<ScalarT>::value,
                                                                    Is_Masked<PixelT>>,
                                                 typename std::negation<std::is_same<PixelT,Pixel_Mask<ScalarT>>>::value>,
                              typename Compound_Result<math::Arg_Val_Sum_Functor<ScalarT>,PixelT>::type>
    operator + ( const Pixel_Base<PixelT>& pixel,
                 Pixel_Mask<ScalarT>       masked_scalar )
{
    if (!masked_scalar.valid())
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( math::Arg_Val_Sum_Functor<ScalarT>( masked_scalar.child() ),
                               px );
    }
    else
    {
        return compound_apply( math::Arg_Val_Sum_Functor<ScalarT>( masked_scalar.child()),
                                                             pixel.impl() );
    }
}

/**
 * Value Argument Addition
*/
template <typename PixelT,
          typename ScalarT>
    typename std::enable_if_t<std::conjunction_v<std::conjunction<math::Is_Scalar<ScalarT>,
                                                                  Is_Masked<PixelT>>,
                                               std::negation<std::is_same<PixelT,Pixel_Mask<ScalarT>>>>,
                              typename Compound_Result<math::Val_Arg_Sum_Functor<ScalarT>,PixelT>::type >
  operator + ( Pixel_Mask<ScalarT>       masked_scalar,
               const Pixel_Base<PixelT>& pixel )
{
    if( !masked_scalar.valid() )
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( math::Val_Arg_Sum_Functor<ScalarT>( masked_scalar.child() ), px );
    }
    else
    { 
        return compound_apply( Val_Arg_Sum_Functor<ScalarT>( masked_scalar.child() ),
                                                             pixel.impl() );
    }
}

/**
 * Addition In-Place Operator
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t<std::conjunction_v<typename math::Is_Scalar<ScalarT>::value,
                                             Is_Masked<PixelT> >,
                 PixelT&>::type
    operator += ( Pixel_Base<PixelT>& pixel,
                  Pixel_Mask<ScalarT> masked_scalar )
{
    if( !masked_scalar.valid() )
    {
        pixel.impl().invalidate();
    }
    return compound_apply_in_place( math::Arg_Val_In_Place_Sum_Functor<ScalarT>( masked_scalar.child()),
                                                                                 pixel.impl() );
}
/**
 * Argument Value Subtraction
*/
 template <typename PixelT,
           typename ScalarT>
    typename std::enable_if<std::conjunction_v<std::conjunction<typename math::Is_Scalar<ScalarT>::value,Is_Masked<PixelT> >,
                               std::negation<std::is_same<PixelT,Pixel_Mask<ScalarT> > > >,
                               typename Compound_Result<math::Arg_Val_Difference_Functor<ScalarT>,PixelT>::type >
  operator * ( const Pixel_Base<PixelT>& pixel,
               Pixel_Mask<ScalarT>       masked_scalar )
{
    if (!masked_scalar.valid())
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( math::Arg_Val_Difference_Functor<ScalarT>( masked_scalar.child() ), px );
    }
    else
    {
        return compound_apply( math::Arg_Val_Difference_Functor<ScalarT>( masked_scalar.child()),
                                                                          pixel.impl() );
    }
}

/**
 * Value Argument Subtraction
*/
template <typename PixelT,
          typename ScalarT>
    typename std::enable_if_t<std::conjunction_v<std::conjunction<typename math::Is_Scalar<ScalarT>::value,
                                                                  Is_Masked<PixelT>>,
                              std::negation<std::is_same<PixelT,Pixel_Mask<ScalarT> > > >,
                              typename Compound_Result<math::Val_Arg_Difference_Functor<ScalarT>,PixelT>::type >
  operator - ( Pixel_Mask<ScalarT>       masked_scalar,
               const Pixel_Base<PixelT>& pixel )
{
    if( !masked_scalar.valid() )
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( math::Val_Arg_Difference_Functor<ScalarT>( masked_scalar.child() ), px );
    }
    else
    { 
        return compound_apply( math::Val_Arg_Difference_Functor<ScalarT>( masked_scalar.child() ),
                                                                          pixel.impl() );
    }
}

/**
 * Subtraction In-Place Operator
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t<std::conjunction_v<typename math::Is_Scalar<ScalarT>::value,
                                             Is_Masked<PixelT> >,
                 PixelT&>
    operator -= ( Pixel_Base<PixelT>& pixel,
                  Pixel_Mask<ScalarT> masked_scalar )
{
    if( !masked_scalar.valid() )
    {
        pixel.impl().invalidate();
    }
    return compound_apply_in_place( math::Arg_Val_In_Place_Difference_Functor<ScalarT>( masked_scalar.child()),
                                                                                        pixel.impl() );
}

/**
 * Argument Value Multiplication
*/
 template <typename PixelT,
           typename ScalarT>
    typename std::enable_if<std::conjunction_v<std::conjunction<typename math::Is_Scalar<ScalarT>::value,
                                                                Is_Masked<PixelT> >,
                                               std::negation<std::is_same<PixelT,Pixel_Mask<ScalarT>>>>,
                            typename Compound_Result<math::Arg_Val_Product_Functor<ScalarT>,PixelT>::type >
  operator * ( const Pixel_Base<PixelT>& pixel,
               Pixel_Mask<ScalarT>       masked_scalar )
{
    if (!masked_scalar.valid())
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( math::Arg_Val_Product_Functor<ScalarT>( masked_scalar.child() ), px );
    }
    else
    {
        return compound_apply( math::Arg_Val_Product_Functor<ScalarT>( masked_scalar.child()),
                                                                       pixel.impl() );
    }
}

/**
 * Value Argument Multiplication
*/
template <typename PixelT,
          typename ScalarT>
    typename std::enable_if_t< std::conjunction_v<std::conjunction<math::Is_Scalar<ScalarT>,
                                                                   Is_Masked<PixelT> >,
                               std::negation<std::is_same<PixelT,
                                                          Pixel_Mask<ScalarT> > > >,
                               typename Compound_Result<math::Val_Arg_Product_Functor<ScalarT>,PixelT>::type >
  operator * ( Pixel_Mask<ScalarT>       masked_scalar,
               const Pixel_Base<PixelT>& pixel )
{
    if( !masked_scalar.valid() )
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( math::Val_Arg_Product_Functor<ScalarT>( masked_scalar.child()), px );
    }
    else
    { 
        return compound_apply( math::Val_Arg_Product_Functor<ScalarT>( masked_scalar.child() ),
                                                                       pixel.impl() );
    }
}


/**
 * Multiplication In-Place Operator
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t<std::conjunction_v<typename math::Is_Scalar<ScalarT>::value,
                                             Is_Masked<PixelT> >,
                          PixelT&>
    operator *= ( Pixel_Base<PixelT>& pixel,
                  Pixel_Mask<ScalarT> masked_scalar )
{
    if( !masked_scalar.valid() )
    {
        pixel.impl().invalidate();
    }
    return compound_apply_in_place( math::Arg_Val_In_Place_Product_Functor<ScalarT>( masked_scalar.child()),
                                                                                     pixel.impl() );
}

/**
 * Argument Value Division
*/
template <typename PixelT,
          typename ScalarT>
    typename std::enable_if<std::conjunction_v<std::conjunction<typename math::Is_Scalar<ScalarT>::value,
                                                                Is_Masked<PixelT> >,
                                             std::negation<std::is_same<PixelT,Pixel_Mask<ScalarT> > > >,
                               typename Compound_Result<math::Arg_Val_Quotient_Functor<ScalarT>,PixelT>::type >
    operator / ( const Pixel_Base<PixelT>& pixel,
                 Pixel_Mask<ScalarT>       masked_scalar )
{
    if (!masked_scalar.valid())
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( math::Arg_Val_Quotient_Functor<ScalarT>(masked_scalar.child()), px );
    }
    else
    {
        return compound_apply( Arg_Val_Quotient_Functor<ScalarT>( masked_scalar.child()),
                                                                  pixel.impl() );
    }
}

/**
 * Value Argument Division
*/
template <typename PixelT,
          typename ScalarT>
    typename std::enable_if_t<std::conjunction_v<std::conjunction<typename math::Is_Scalar<ScalarT>::value,
                                                                  Is_Masked<PixelT>>,
                               std::negation<std::is_same<PixelT,Pixel_Mask<ScalarT>> > >,
                               typename Compound_Result<math::Val_Arg_Quotient_Functor<ScalarT>,PixelT>::type >
  operator / ( Pixel_Mask<ScalarT>       masked_scalar,
               const Pixel_Base<PixelT>& pixel )
{
    if( !masked_scalar.valid() )
    {
        PixelT px = pixel.impl();
        px.invalidate();
        return compound_apply( Val_Arg_Quotient_Functor<ScalarT>( masked_scalar.child() ), px );
    }
    else
    { 
        return compound_apply( Val_Arg_Quotient_Functor<ScalarT>( masked_scalar.child() ),
                               pixel.impl() );
    }
}


/**
 * Division In-Place Operator
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t<std::conjunction_v<typename math::Is_Scalar<ScalarT>::value,
                                             Is_Masked<PixelT> >,
                          PixelT&>
    operator /=( Pixel_Base<PixelT>& pixel,
                 Pixel_Mask<ScalarT> masked_scalar )
{
    if( !masked_scalar.valid() )
    {
        pixel.impl().invalidate();
    }
    return compound_apply_in_place( Arg_Val_In_Place_Quotient_Functor<ScalarT>( masked_scalar.child()),
                                                                                pixel.impl() );
}

} // End of image namespace
} // End of tmns namespace