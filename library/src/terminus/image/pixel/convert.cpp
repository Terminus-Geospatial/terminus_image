/**
 * @file    convert.cpp
 * @author  Marvin Smith
 * @date    7/16/2023
*/
#include "convert.hpp"

// C++ Libraries
#include <map>
#include <tuple>

// Boost Libraries
#include <boost/integer_traits.hpp>

// Terminus Libraries
#include "Channel_Conversion_Utilities.hpp"
#include "Channel_Type_ID.hpp"

// External Terminus Libraries
#include <terminus/log/utility.hpp>

namespace tmns::image {

// -----------------------------------------------------------------
// Section for handling type conversion


/// Declare function type: Convert a src value to a dest value
typedef void (*channel_convert_func)(void* src, void* dest);

/// Apply a cast from the src to the dest types
template <class SrcT, class DestT>
void channel_convert_cast( SrcT* src, DestT* dest ) {
  *dest = DestT(*src);
}

// Simple integer type conversion functions
void channel_convert_uint16_to_uint8( uint16_t* src, uint8_t* dest )
{
  *dest = uint8_t( *src / (65535/255) );
}
void channel_convert_uint8_to_uint16( uint8_t* src, uint16_t* dest )
{
  *dest = uint16_t( *src ) * (65535/255);
}

/// Convert a float in the range -1 to +1 to an integer type.
template <class SrcT, class DestT>
void channel_convert_float_to_int( SrcT* src, DestT* dest )
{
  if( *src > SrcT(1.0) ) *dest = boost::integer_traits<DestT>::const_max;
  else if( *src < SrcT(0.0) ) *dest = DestT(0);
  else *dest = DestT( *src * boost::integer_traits<DestT>::const_max );
}

/// Pointers to two maps:  <type pair> -> conversion function
/// - One is for rescaling conversions, the other for non-rescaling.
std::map< std::pair< Channel_Type_Enum, Channel_Type_Enum >, channel_convert_func >*  channel_convert_map         = nullptr;
std::map< std::pair< Channel_Type_Enum, Channel_Type_Enum >, channel_convert_func >*  channel_convert_rescale_map = nullptr;

/// Helper class for adding entries to the two conversion maps mentioned above.
class Channel_Convert_Map_Entry
{
    private:

        /// Instantiate the two channel conversion maps mentioned above if they do not exist.
        void initialize()
        {
            if( !channel_convert_map )
            {
                channel_convert_map = new std::map< std::pair< Channel_Type_Enum,
                                                               Channel_Type_Enum>,
                                                    channel_convert_func >();
            }
            if( !channel_convert_rescale_map )
            {
              channel_convert_rescale_map = new std::map<std::pair<Channel_Type_Enum,
                                                                   Channel_Type_Enum>,
                                                         channel_convert_func>();
            }
        }

    public:

        /// Load a non-scaling conversion function into the two maps (the same function goes in both).
        template <class SrcT, class DstT>
        Channel_Convert_Map_Entry( void (*func)(SrcT*,DstT*) )
        {
            initialize();
            Channel_Type_Enum src = Channel_Type_ID<SrcT>::value;
            Channel_Type_Enum dst = Channel_Type_ID<DstT>::value;
            channel_convert_map        ->operator[]( std::make_pair(src,dst) ) = (channel_convert_func)func;
            channel_convert_rescale_map->operator[]( std::make_pair(src,dst) ) = (channel_convert_func)func;
        }

        /// Load both a non-scaling and rescaling function into the two maps.
        template <class SrcT, class DstT>
        Channel_Convert_Map_Entry( void (*func)(SrcT*,DstT*), void (*rescale_func)(SrcT*,DstT*) )
        {
            initialize();
            Channel_Type_Enum src = Channel_Type_ID<SrcT>::value;
            Channel_Type_Enum dst = Channel_Type_ID<DstT>::value;
            channel_convert_map        ->operator[]( std::make_pair(src,dst) ) = (channel_convert_func)func;
            channel_convert_rescale_map->operator[]( std::make_pair(src,dst) ) = (channel_convert_func)rescale_func;
        }
};

// Load up a bunch of generic conversion functions!
// - Might be better to use a default handler for the most common case.
Channel_Convert_Map_Entry  _conv_i8i8  ( &channel_convert_cast<   int8_t,   int8_t>   );
Channel_Convert_Map_Entry  _conv_i8u8  ( &channel_convert_cast<   int8_t,  uint8_t>  );
Channel_Convert_Map_Entry  _conv_i8i16 ( &channel_convert_cast<   int8_t,  int16_t>  );
Channel_Convert_Map_Entry  _conv_i8u16 ( &channel_convert_cast<   int8_t, uint16_t> );
Channel_Convert_Map_Entry  _conv_i8i32 ( &channel_convert_cast<   int8_t,  int32_t>  );
Channel_Convert_Map_Entry  _conv_i8u32 ( &channel_convert_cast<   int8_t, uint32_t> );
Channel_Convert_Map_Entry  _conv_i8i64 ( &channel_convert_cast<   int8_t,  int64_t>  );
Channel_Convert_Map_Entry  _conv_i8u64 ( &channel_convert_cast<   int8_t, uint64_t> );
Channel_Convert_Map_Entry  _conv_i8f32 ( &channel_convert_cast<   int8_t,    float>, &channel_convert_int_to_float<int8_t,float>  );
Channel_Convert_Map_Entry  _conv_i8f64 ( &channel_convert_cast<   int8_t,   double>, &channel_convert_int_to_float<int8_t,double> );
Channel_Convert_Map_Entry  _conv_u8i8  ( &channel_convert_cast<  uint8_t,   int8_t>   );
Channel_Convert_Map_Entry  _conv_u8u8  ( &channel_convert_cast<  uint8_t,  uint8_t>  );
Channel_Convert_Map_Entry  _conv_u8i16 ( &channel_convert_cast<  uint8_t,  int16_t>  );
Channel_Convert_Map_Entry  _conv_u8u16 ( &channel_convert_cast<  uint8_t, uint16_t>, &channel_convert_uint8_to_uint16 );
Channel_Convert_Map_Entry  _conv_u8i32 ( &channel_convert_cast<  uint8_t,  int32_t>  );
Channel_Convert_Map_Entry  _conv_u8u32 ( &channel_convert_cast<  uint8_t, uint32_t> );
Channel_Convert_Map_Entry  _conv_u8i64 ( &channel_convert_cast<  uint8_t,  int64_t>  );
Channel_Convert_Map_Entry  _conv_u8u64 ( &channel_convert_cast<  uint8_t, uint64_t> );
Channel_Convert_Map_Entry  _conv_u8f32 ( &channel_convert_cast<  uint8_t,    float>, &channel_convert_int_to_float<uint8_t,float>  );
Channel_Convert_Map_Entry  _conv_u8f64 ( &channel_convert_cast<  uint8_t,   double>, &channel_convert_int_to_float<uint8_t,double> );
Channel_Convert_Map_Entry  _conv_i16i8 ( &channel_convert_cast<  int16_t,   int8_t>   );
Channel_Convert_Map_Entry  _conv_i16u8 ( &channel_convert_cast<  int16_t,  uint8_t>  );
Channel_Convert_Map_Entry  _conv_i16i16( &channel_convert_cast<  int16_t,  int16_t>  );
Channel_Convert_Map_Entry  _conv_i16u16( &channel_convert_cast<  int16_t, uint16_t> );
Channel_Convert_Map_Entry  _conv_i16i32( &channel_convert_cast<  int16_t,  int32_t>  );
Channel_Convert_Map_Entry  _conv_i16u32( &channel_convert_cast<  int16_t, uint32_t> );
Channel_Convert_Map_Entry  _conv_i16i64( &channel_convert_cast<  int16_t,  int64_t>  );
Channel_Convert_Map_Entry  _conv_i16u64( &channel_convert_cast<  int16_t, uint64_t> );
Channel_Convert_Map_Entry  _conv_i16f32( &channel_convert_cast<  int16_t,    float>, &channel_convert_int_to_float<int16_t,float>  );
Channel_Convert_Map_Entry  _conv_i16f64( &channel_convert_cast<  int16_t,   double>, &channel_convert_int_to_float<int16_t,double> );
Channel_Convert_Map_Entry  _conv_u16i8 ( &channel_convert_cast< uint16_t,    int8_t>   );
Channel_Convert_Map_Entry  _conv_u16u8 ( &channel_convert_cast< uint16_t,   uint8_t>, &channel_convert_uint16_to_uint8 );
Channel_Convert_Map_Entry  _conv_u16i16( &channel_convert_cast< uint16_t,   int16_t>  );
Channel_Convert_Map_Entry  _conv_u16u16( &channel_convert_cast< uint16_t,  uint16_t> );
Channel_Convert_Map_Entry  _conv_u16i32( &channel_convert_cast< uint16_t,   int32_t>  );
Channel_Convert_Map_Entry  _conv_u16u32( &channel_convert_cast< uint16_t,  uint32_t> );
Channel_Convert_Map_Entry  _conv_u16i64( &channel_convert_cast< uint16_t,   int64_t>  );
Channel_Convert_Map_Entry  _conv_u16u64( &channel_convert_cast< uint16_t,  uint64_t> );
Channel_Convert_Map_Entry  _conv_u16f32( &channel_convert_cast< uint16_t,     float>, &channel_convert_int_to_float<uint16_t,float>  );
Channel_Convert_Map_Entry  _conv_u16f64( &channel_convert_cast< uint16_t,    double>, &channel_convert_int_to_float<uint16_t,double> );
Channel_Convert_Map_Entry  _conv_i32i8 ( &channel_convert_cast<  int32_t,    int8_t>   );
Channel_Convert_Map_Entry  _conv_i32u8 ( &channel_convert_cast<  int32_t,   uint8_t>  );
Channel_Convert_Map_Entry  _conv_i32i16( &channel_convert_cast<  int32_t,   int16_t>  );
Channel_Convert_Map_Entry  _conv_i32u16( &channel_convert_cast<  int32_t,  uint16_t> );
Channel_Convert_Map_Entry  _conv_i32i32( &channel_convert_cast<  int32_t,   int32_t>  );
Channel_Convert_Map_Entry  _conv_i32u32( &channel_convert_cast<  int32_t,  uint32_t> );
Channel_Convert_Map_Entry  _conv_i32i64( &channel_convert_cast<  int32_t,   int64_t>  );
Channel_Convert_Map_Entry  _conv_i32u64( &channel_convert_cast<  int32_t,  uint64_t> );
Channel_Convert_Map_Entry  _conv_i32f32( &channel_convert_cast<  int32_t,     float>, &channel_convert_int_to_float<int32_t,float>  );
Channel_Convert_Map_Entry  _conv_i32f64( &channel_convert_cast<  int32_t,    double>, &channel_convert_int_to_float<int32_t,double> );
Channel_Convert_Map_Entry  _conv_u32i8 ( &channel_convert_cast< uint32_t,    int8_t>   );
Channel_Convert_Map_Entry  _conv_u32u8 ( &channel_convert_cast< uint32_t,   uint8_t>  );
Channel_Convert_Map_Entry  _conv_u32i16( &channel_convert_cast< uint32_t,   int16_t>  );
Channel_Convert_Map_Entry  _conv_u32u16( &channel_convert_cast< uint32_t,  uint16_t> );
Channel_Convert_Map_Entry  _conv_u32i32( &channel_convert_cast< uint32_t,   int32_t>  );
Channel_Convert_Map_Entry  _conv_u32u32( &channel_convert_cast< uint32_t,  uint32_t> );
Channel_Convert_Map_Entry  _conv_u32i64( &channel_convert_cast< uint32_t,   int64_t>  );
Channel_Convert_Map_Entry  _conv_u32u64( &channel_convert_cast< uint32_t,  uint64_t> );
Channel_Convert_Map_Entry  _conv_u32f32( &channel_convert_cast< uint32_t,     float>, &channel_convert_int_to_float<uint32_t,float>  );
Channel_Convert_Map_Entry  _conv_u32f64( &channel_convert_cast< uint32_t,    double>, &channel_convert_int_to_float<uint32_t,double> );
Channel_Convert_Map_Entry  _conv_i64i8 ( &channel_convert_cast<  int64_t,    int8_t>   );
Channel_Convert_Map_Entry  _conv_i64u8 ( &channel_convert_cast<  int64_t,   uint8_t>  );
Channel_Convert_Map_Entry  _conv_i64i16( &channel_convert_cast<  int64_t,   int16_t>  );
Channel_Convert_Map_Entry  _conv_i64u16( &channel_convert_cast<  int64_t,  uint16_t> );
Channel_Convert_Map_Entry  _conv_i64i32( &channel_convert_cast<  int64_t,   int32_t>  );
Channel_Convert_Map_Entry  _conv_i64u32( &channel_convert_cast<  int64_t,  uint32_t> );
Channel_Convert_Map_Entry  _conv_i64i64( &channel_convert_cast<  int64_t,   int64_t>  );
Channel_Convert_Map_Entry  _conv_i64u64( &channel_convert_cast<  int64_t,  uint64_t> );
Channel_Convert_Map_Entry  _conv_i64f32( &channel_convert_cast<  int64_t,     float>, &channel_convert_int_to_float<int64_t,float>  );
Channel_Convert_Map_Entry  _conv_i64f64( &channel_convert_cast<  int64_t,    double>, &channel_convert_int_to_float<int64_t,double> );
Channel_Convert_Map_Entry  _conv_u64i8 ( &channel_convert_cast< uint64_t,    int8_t>   );
Channel_Convert_Map_Entry  _conv_u64u8 ( &channel_convert_cast< uint64_t,   uint8_t>  );
Channel_Convert_Map_Entry  _conv_u64i16( &channel_convert_cast< uint64_t,   int16_t>  );
Channel_Convert_Map_Entry  _conv_u64u16( &channel_convert_cast< uint64_t,  uint16_t> );
Channel_Convert_Map_Entry  _conv_u64i32( &channel_convert_cast< uint64_t,   int32_t>  );
Channel_Convert_Map_Entry  _conv_u64u32( &channel_convert_cast< uint64_t,  uint32_t> );
Channel_Convert_Map_Entry  _conv_u64i64( &channel_convert_cast< uint64_t,   int64_t>  );
Channel_Convert_Map_Entry  _conv_u64u64( &channel_convert_cast< uint64_t,  uint64_t> );
Channel_Convert_Map_Entry  _conv_u64f32( &channel_convert_cast< uint64_t,     float>, &channel_convert_int_to_float<uint64_t,float > );
Channel_Convert_Map_Entry  _conv_u64f64( &channel_convert_cast< uint64_t,    double>, &channel_convert_int_to_float<uint64_t,double> );
Channel_Convert_Map_Entry  _conv_f32i8 ( &channel_convert_cast<    float,    int8_t>, &channel_convert_float_to_int<float, int8_t>  );
Channel_Convert_Map_Entry  _conv_f32u8 ( &channel_convert_cast<    float,   uint8_t>, &channel_convert_float_to_int<float,uint8_t>  );
Channel_Convert_Map_Entry  _conv_f32i16( &channel_convert_cast<    float,   int16_t>, &channel_convert_float_to_int<float, int16_t> );
Channel_Convert_Map_Entry  _conv_f32u16( &channel_convert_cast<    float,  uint16_t>, &channel_convert_float_to_int<float,uint16_t> );
Channel_Convert_Map_Entry  _conv_f32i32( &channel_convert_cast<    float,   int32_t>, &channel_convert_float_to_int<float, int32_t> );
Channel_Convert_Map_Entry  _conv_f32u32( &channel_convert_cast<    float,  uint32_t>, &channel_convert_float_to_int<float,uint32_t> );
Channel_Convert_Map_Entry  _conv_f32i64( &channel_convert_cast<    float,   int64_t>, &channel_convert_float_to_int<float, int64_t> );
Channel_Convert_Map_Entry  _conv_f32u64( &channel_convert_cast<    float,  uint64_t>, &channel_convert_float_to_int<float,uint64_t> );
Channel_Convert_Map_Entry  _conv_f32f32( &channel_convert_cast<    float,     float> );
Channel_Convert_Map_Entry  _conv_f32f64( &channel_convert_cast<    float,    double> );
Channel_Convert_Map_Entry  _conv_f64i8 ( &channel_convert_cast<   double,    int8_t>, &channel_convert_float_to_int<double, int8_t>  );
Channel_Convert_Map_Entry  _conv_f64u8 ( &channel_convert_cast<   double,   uint8_t>, &channel_convert_float_to_int<double,uint8_t>  );
Channel_Convert_Map_Entry  _conv_f64i16( &channel_convert_cast<   double,   int16_t>, &channel_convert_float_to_int<double, int16_t> );
Channel_Convert_Map_Entry  _conv_f64u16( &channel_convert_cast<   double,  uint16_t>, &channel_convert_float_to_int<double,uint16_t> );
Channel_Convert_Map_Entry  _conv_f64i32( &channel_convert_cast<   double,   int32_t>, &channel_convert_float_to_int<double, int32_t> );
Channel_Convert_Map_Entry  _conv_f64u32( &channel_convert_cast<   double,  uint32_t>, &channel_convert_float_to_int<double,uint32_t> );
Channel_Convert_Map_Entry  _conv_f64i64( &channel_convert_cast<   double,   int64_t>, &channel_convert_float_to_int<double, int64_t> );
Channel_Convert_Map_Entry  _conv_f64u64( &channel_convert_cast<   double,  uint64_t>, &channel_convert_float_to_int<double,uint64_t> );
Channel_Convert_Map_Entry  _conv_f64f32( &channel_convert_cast<   double,     float> );
Channel_Convert_Map_Entry  _conv_f64f64( &channel_convert_cast<   double,    double> );


//------------------------------------------------------------------------------------
// Section for assigning max value

// Each of these sections of code can be replaced with simpler functions
//  incorporating a switch statement: ChannelTypeEnum -> function call

// Function type: Assigns a channel the maximum value
typedef void ( *channel_set_max_func )( void* dest );

// Implementation for any integer
template <class DestT>
void channel_set_max_int( DestT* dest )
{
    *dest = boost::integer_traits<DestT>::const_max;
}


// Implementation for any float type
template <class DestT>
void channel_set_max_float( DestT* dest )
{
    *dest = DestT(1.0);
}

// The map, class, and entry setting mirror the previous section.

std::map<Channel_Type_Enum, channel_set_max_func>*  channel_set_max_map = 0;

class Channel_Set_Max_Map_Entry
{
    public:
        template <class DstT>
        Channel_Set_Max_Map_Entry( void (*func)(DstT*) )
        {
            if( !channel_set_max_map )
            {
                channel_set_max_map = new std::map<Channel_Type_Enum, channel_set_max_func>();
            }
            Channel_Type_Enum dst = Channel_Type_ID<DstT>::value;
            channel_set_max_map->operator[]( dst ) = (channel_set_max_func)func;
        }
};

Channel_Set_Max_Map_Entry _setmax_i8 ( &channel_set_max_int<int8_t  > );
Channel_Set_Max_Map_Entry _setmax_u8 ( &channel_set_max_int<uint8_t > );
Channel_Set_Max_Map_Entry _setmax_i16( &channel_set_max_int<int16_t > );
Channel_Set_Max_Map_Entry _setmax_u16( &channel_set_max_int<uint16_t> );
Channel_Set_Max_Map_Entry _setmax_i32( &channel_set_max_int<int32_t > );
Channel_Set_Max_Map_Entry _setmax_u32( &channel_set_max_int<uint32_t> );
Channel_Set_Max_Map_Entry _setmax_i64( &channel_set_max_int<int64_t > );
Channel_Set_Max_Map_Entry _setmax_u64( &channel_set_max_int<uint64_t> );
Channel_Set_Max_Map_Entry _setmax_f32( &channel_set_max_float<float > );
Channel_Set_Max_Map_Entry _setmax_f64( &channel_set_max_float<double> );

//------------------------------------------------------------------------------------
// Section for averaging channels

// Channel Average:
//   Reduces a number of channels into one by averaging
typedef void (*channel_average_func)(void* src, void* dest, int32_t len);

template <class T>
void channel_average( T* src, T* dest, int len )
{
    typename Accumulator_Type<T>::type accum = typename Accumulator_Type<T>::type();
    for( int32_t i=0; i<len; ++i ) accum += src[i];
    *dest = accum / len;
}

// The map, class, and entry setting mirror the previous section.
std::map<Channel_Type_Enum,channel_average_func>*  channel_average_map = 0;

class Channel_Average_Map_Entry
{
    public:

        template <class T>
        Channel_Average_Map_Entry( void (*func)( T*, T*, int ) )
        {
            if( !channel_average_map )
            {
                channel_average_map = new std::map<Channel_Type_Enum, channel_average_func>();
            }
            Channel_Type_Enum ctid = Channel_Type_ID<T>::value;
            channel_average_map->operator[]( ctid ) = (channel_average_func)func;
        }
};

Channel_Average_Map_Entry _average_i8 ( &channel_average<int8_t> );
Channel_Average_Map_Entry _average_u8 ( &channel_average<uint8_t> );
Channel_Average_Map_Entry _average_i16( &channel_average<int16_t> );
Channel_Average_Map_Entry _average_u16( &channel_average<uint16_t> );
Channel_Average_Map_Entry _average_i32( &channel_average<int32_t> );
Channel_Average_Map_Entry _average_u32( &channel_average<uint32_t> );
Channel_Average_Map_Entry _average_i64( &channel_average<int64_t> );
Channel_Average_Map_Entry _average_u64( &channel_average<uint64_t> );
Channel_Average_Map_Entry _average_f32( &channel_average<float> );
Channel_Average_Map_Entry _average_f64( &channel_average<double> );


//------------------------------------------------------------------------------------
// Premultiply section

// Channel Premultiply:
//   Applies the Alpha Channel to the rest of the channels:
typedef void (*channel_premultiply_func)(void* src, void* dst, int len);

template <class T>
void channel_premultiply_int( T* src, T* dst, int32_t len )
{
    double scale = src[len-1] / (double)(boost::integer_traits<T>::const_max);
    for( int i=0; i<len-1; ++i ) dst[i] = T( round(src[i] * scale) );
    dst[len-1] = src[len-1];
}

template <class T>
void channel_premultiply_float( T* src, T* dst, int len )
{
    double scale = (double)(src[len-1]);
    for( int i=0; i<len-1; ++i ) dst[i] = T( src[i] * scale );
    dst[len-1] = src[len-1];
}

// The map, class, and entry setting mirror the previous section.

std::map<Channel_Type_Enum,channel_premultiply_func>* channel_premultiply_map = 0;

class Channel_Premultiply_Map_Entry
{
      public:
      template <class T>
      Channel_Premultiply_Map_Entry( void (*func)( T*, T*, int32_t ) )
      {
        if( !channel_premultiply_map )
        {
            channel_premultiply_map = new std::map<Channel_Type_Enum,channel_premultiply_func>();
        }
        Channel_Type_Enum ctid = Channel_Type_ID<T>::value;
        channel_premultiply_map->operator[]( ctid ) = (channel_premultiply_func)func;
    }
};

Channel_Premultiply_Map_Entry _premultiply_i8 ( &channel_premultiply_int<int8_t> );
Channel_Premultiply_Map_Entry _premultiply_u8 ( &channel_premultiply_int<uint8_t> );
Channel_Premultiply_Map_Entry _premultiply_i16( &channel_premultiply_int<int16_t> );
Channel_Premultiply_Map_Entry _premultiply_u16( &channel_premultiply_int<uint16_t> );
Channel_Premultiply_Map_Entry _premultiply_i32( &channel_premultiply_int<int32_t> );
Channel_Premultiply_Map_Entry _premultiply_u32( &channel_premultiply_int<uint32_t> );
Channel_Premultiply_Map_Entry _premultiply_i64( &channel_premultiply_int<int64_t> );
Channel_Premultiply_Map_Entry _premultiply_u64( &channel_premultiply_int<uint64_t> );
Channel_Premultiply_Map_Entry _premultiply_f32( &channel_premultiply_float<float> );
Channel_Premultiply_Map_Entry _premultiply_f64( &channel_premultiply_float<double> );

//-----------------------------------------------------------------
// Unpremultiply section

/// Channel Unpremultiply:
///   Removes the premultiply of alpha to other channels:
typedef void (*channel_unpremultiply_func)( void* src, void* dst, int len );

template <class T>
void channel_unpremultiply_int( T* src, T* dst, int len )
{
    double scale = src[len-1] / (double)(boost::integer_traits<T>::const_max);
    for( int i=0; i<len-1; ++i ) dst[i] = T( round(src[i] / scale) );
    dst[len-1] = src[len-1];
}

template <class T>
void channel_unpremultiply_float( T* src, T* dst, int len )
{
    double scale = (double)(src[len-1]);
    for( int i=0; i<len-1; ++i ) dst[i] = T( src[i] / scale );
    dst[len-1] = src[len-1];
}

// The map, class, and entry setting mirror the previous section.

std::map< Channel_Type_Enum,
          channel_unpremultiply_func >* channel_unpremultiply_map = 0;

class Channel_Unpremultiply_Map_Entry
{
    public:

      template <class T>
      Channel_Unpremultiply_Map_Entry( void (*func)( T*, T*, int32_t ) )
      {
          if( !channel_unpremultiply_map )
          {
              channel_unpremultiply_map = new std::map<Channel_Type_Enum,channel_unpremultiply_func>();
          }
          Channel_Type_Enum ctid = Channel_Type_ID<T>::value;
          channel_unpremultiply_map->operator[]( ctid ) = (channel_unpremultiply_func)func;
      }
};

Channel_Unpremultiply_Map_Entry _unpremultiply_i8 ( &channel_unpremultiply_int<int8_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_u8 ( &channel_unpremultiply_int<uint8_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_i16( &channel_unpremultiply_int<int16_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_u16( &channel_unpremultiply_int<uint16_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_i32( &channel_unpremultiply_int<int32_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_u32( &channel_unpremultiply_int<uint32_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_i64( &channel_unpremultiply_int<int64_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_u64( &channel_unpremultiply_int<uint64_t> );
Channel_Unpremultiply_Map_Entry _unpremultiply_f32( &channel_unpremultiply_float<float> );
Channel_Unpremultiply_Map_Entry _unpremultiply_f64( &channel_unpremultiply_float<double> );


/****************************************/
/*          Convert Pixel Data          */
/****************************************/
Result<void> convert( const Image_Buffer&  dst,
                      const Image_Buffer&  src,
                      bool                 rescale )
{
    // Check ranges and other good stuff
    if( dst.format().cols() != src.format().cols() ||
        dst.format().rows() != src.format().rows() )
    {
        return outcome::fail( core::error::ErrorCode::INVALID_CONFIGURATION,
                              "Destination buffer has incorrect size." );
    }

    // If pixel types are the same, then it's a channel conversion
    if( dst.format().pixel_type() != src.format().pixel_type() )
    {
        // Do nothing for now, it's just a check
    }

    //tmns::log::info( "Destination Buffer:\n", dst.to_string() );

    /**
     * We only support a few special conversions, and the general case where
     * the source and destination formats are the same.  Below we assume that
     * we're doing a supported conversion, so we check first.
    */
   if( dst.format().pixel_type() != src.format().pixel_type() )
   {
        // We freely convert between multi-channel and multi-plane images,
        // by aliasing the multi-channel buffer as a multi-plane buffer.
        if( src.format().pixel_type() == Pixel_Format_Enum::SCALAR &&
            dst.format().planes()     == 1  &&
            src.format().planes()     == num_channels( dst.format().pixel_type() ).value() )
        {
            Image_Buffer new_dst          = dst;
            new_dst.format().set_pixel_type( Pixel_Format_Enum::SCALAR );
            new_dst.format().set_planes( src.format().planes() );
            new_dst.set_pstride( channel_size_bytes( dst.format().channel_type() ).value() );
            return convert( new_dst, src );
        }
        else if( dst.format().pixel_type() == Pixel_Format_Enum::SCALAR &&
                 src.format().planes()     == 1 &&
                 dst.format().planes()     == num_channels( src.format().pixel_type() ).value() )
        {
            Image_Buffer new_src          = src;
            new_src.format().set_pixel_type( Pixel_Format_Enum::SCALAR );
            new_src.format().set_planes( dst.format().planes() );
            new_src.set_pstride( channel_size_bytes( src.format().channel_type() ).value() );
            return convert( dst, new_src );
        }

        // We support conversions between user specified generic pixel
        // types and the pixel types with an identical number of channels.
        if ( ( src.format().pixel_type() == Pixel_Format_Enum::SCALAR_MASKED     && dst.format().pixel_type() == Pixel_Format_Enum::GRAYA ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::SCALAR_MASKED     && src.format().pixel_type() == Pixel_Format_Enum::GRAYA ) ||
             ( src.format().pixel_type() == Pixel_Format_Enum::GRAY_MASKED       && dst.format().pixel_type() == Pixel_Format_Enum::GRAYA ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::GRAY_MASKED       && src.format().pixel_type() == Pixel_Format_Enum::GRAYA ) ||
             ( src.format().pixel_type() == Pixel_Format_Enum::RGB_MASKED        && dst.format().pixel_type() == Pixel_Format_Enum::RGBA  ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::RGB_MASKED        && src.format().pixel_type() == Pixel_Format_Enum::RGBA  ) ||
             ( src.format().pixel_type() == Pixel_Format_Enum::GENERIC_1_CHANNEL && dst.format().pixel_type() == Pixel_Format_Enum::GRAY  ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::GENERIC_1_CHANNEL && src.format().pixel_type() == Pixel_Format_Enum::GRAY  ) ||
             ( src.format().pixel_type() == Pixel_Format_Enum::GENERIC_2_CHANNEL && dst.format().pixel_type() == Pixel_Format_Enum::GRAYA ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::GENERIC_2_CHANNEL && src.format().pixel_type() == Pixel_Format_Enum::GRAYA ) ||
             ( src.format().pixel_type() == Pixel_Format_Enum::GENERIC_3_CHANNEL && dst.format().pixel_type() == Pixel_Format_Enum::RGB   ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::GENERIC_3_CHANNEL && src.format().pixel_type() == Pixel_Format_Enum::RGB   ) ||
             ( src.format().pixel_type() == Pixel_Format_Enum::GENERIC_3_CHANNEL && dst.format().pixel_type() == Pixel_Format_Enum::XYZ   ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::GENERIC_3_CHANNEL && src.format().pixel_type() == Pixel_Format_Enum::XYZ   ) ||
             ( src.format().pixel_type() == Pixel_Format_Enum::GENERIC_4_CHANNEL && dst.format().pixel_type() == Pixel_Format_Enum::RGBA  ) ||
             ( dst.format().pixel_type() == Pixel_Format_Enum::GENERIC_4_CHANNEL && src.format().pixel_type() == Pixel_Format_Enum::RGBA  ) )
        {
            // Do nothing, these combinations are ok to convert.
        }
        // Other than that, we only support conversion between the core pixel formats
        else if( ( src.format().pixel_type() != Pixel_Format_Enum::GRAY  &&
                   src.format().pixel_type() != Pixel_Format_Enum::GRAYA &&
                   src.format().pixel_type() != Pixel_Format_Enum::RGB   &&
                   src.format().pixel_type() != Pixel_Format_Enum::RGBA  &&
                   src.format().pixel_type() != Pixel_Format_Enum::XYZ ) ||
                 ( dst.format().pixel_type() != Pixel_Format_Enum::GRAY &&
                   dst.format().pixel_type() != Pixel_Format_Enum::GRAYA &&
                   dst.format().pixel_type() != Pixel_Format_Enum::RGB  &&
                   dst.format().pixel_type() != Pixel_Format_Enum::RGBA  &&
                   dst.format().pixel_type() != Pixel_Format_Enum::XYZ ) )
        {
            std::stringstream sout;
            sout << "Source and destination buffers have incompatible pixel formats. Source: "
                 << enum_to_string( src.format().pixel_type() ) << " vs. "
                 << enum_to_string( dst.format().pixel_type() ) << ").";
            tmns::log::error( sout.str() );
            return outcome::fail( core::error::ErrorCode::INVALID_PIXEL_TYPE,
                                  sout.str() );
        }
    }

    // Gather some stats
    size_t src_channels = num_channels( src.format().pixel_type() ).value();
    size_t dst_channels = num_channels( dst.format().pixel_type() ).value();
    size_t src_chstride = channel_size_bytes( src.format().channel_type() ).value();
    size_t dst_chstride = channel_size_bytes( dst.format().channel_type() ).value();

    size_t copy_length = ( src_channels == dst_channels ) ?
                          src_channels : ( src_channels<3 ) ? 1 : (dst_channels>=3) ? 3 : 0;

    // Decide how alpha handling and other issues will be done
    bool unpremultiply_src = false;
    bool premultiply_src = false;
    bool premultiply_dst = false;
    {
        const Image_Format& srcf = src.format();
        const Image_Format& dstf = dst.format();

        bool src_alpha = ( srcf.pixel_type() == Pixel_Format_Enum::GRAYA || dstf.pixel_type() == Pixel_Format_Enum::RGBA );
        bool dst_alpha = ( dstf.pixel_type() == Pixel_Format_Enum::GRAYA || dstf.pixel_type() == Pixel_Format_Enum::RGBA );
        unpremultiply_src = ( src_alpha && srcf.premultiply() && !dstf.premultiply() );
        premultiply_src   = ( src_alpha && !dst_alpha && !srcf.premultiply() );
        premultiply_dst   = ( src_alpha && dst_alpha && !srcf.premultiply()  && dstf.premultiply() );
    }

    bool triplicate = src_channels <  3    && dst_channels >= 3;
    bool average    = src_channels >= 3    && dst_channels <  3;
    bool add_alpha  = src_channels % 2 ==1 && dst_channels %  2 == 0;
    bool copy_alpha = src_channels != dst_channels && src_channels % 2 == 0 && dst_channels % 2 == 0;

    // Get handler functions for all of the input data types.
    // - This could be replaced with function calls containing a switch statement.
    channel_convert_func conv_func = rescale ?
                      channel_convert_rescale_map->operator[](
                                                       std::make_pair( src.format().channel_type(),
                                                                       dst.format().channel_type() ) )
                    : channel_convert_map->operator[]( std::make_pair( src.format().channel_type(),
                                                                       dst.format().channel_type() ) );

    channel_set_max_func  max_func                    = channel_set_max_map->operator[]( dst.format().channel_type() );
    channel_average_func  avg_func                    = channel_average_map->operator[]( dst.format().channel_type() );
    channel_unpremultiply_func unpremultiply_src_func = channel_unpremultiply_map->operator[]( src.format().channel_type() );
    channel_premultiply_func   premultiply_src_func   = channel_premultiply_map->operator[]( src.format().channel_type() );
    channel_premultiply_func   premultiply_dst_func   = channel_premultiply_map->operator[]( dst.format().channel_type() );

    if( !conv_func || !max_func || !avg_func || !unpremultiply_src_func || !premultiply_dst_func || !premultiply_src_func )
    {
        return outcome::fail( core::error::ErrorCode::INVALID_CHANNEL_TYPE,
                              "Unsupported channel-type combination in conversion ( ", src.format().channel_type(),
                              " -> ", dst.format().channel_type(), " )" );
    }

    int max_channels = std::max( src_channels, dst_channels );

    std::shared_ptr<uint8_t[]> src_buf(new uint8_t[ max_channels * src_chstride ] );
    std::shared_ptr<uint8_t[]> dst_buf(new uint8_t[ max_channels * dst_chstride ] );

    // Loop through all of the pixels in the source data
    // - Data pointers are always in bytes, will be advanced according to data element size.
    uint8_t*  src_ptr_p = (uint8_t*)src.data();
    uint8_t*  dst_ptr_p = (uint8_t*)dst.data();

    for( uint32_t p=0; p < src.format().planes(); ++p )
    {
        uint8_t *src_ptr_r = src_ptr_p;
        uint8_t *dst_ptr_r = dst_ptr_p;
        for( uint32_t r=0; r<src.format().rows(); ++r )
        {
            uint8_t *src_ptr_c = src_ptr_r;
            uint8_t *dst_ptr_c = dst_ptr_r;
            for( uint32_t c=0; c < src.format().cols(); ++c )
            {
                // Setup the buffers, adjusting premultiplication if needed
                uint8_t *src_ptr = src_ptr_c;
                uint8_t *dst_ptr = dst_ptr_c;
                if( unpremultiply_src )
                {
                    unpremultiply_src_func( src_ptr, src_buf.get(), src_channels );
                    src_ptr = src_buf.get();
                }
                else if( premultiply_src )
                {
                    premultiply_src_func( src_ptr, src_buf.get(), src_channels );
                    src_ptr = src_buf.get();
                }

                // Copy/convert, unrolling the common multi-channel cases
                if( copy_length == 4 )
                {
                    conv_func( src_ptr,                dst_ptr );
                    conv_func( src_ptr+  src_chstride, dst_ptr+  dst_chstride );
                    conv_func( src_ptr+2*src_chstride, dst_ptr+2*dst_chstride );
                    conv_func( src_ptr+3*src_chstride, dst_ptr+3*dst_chstride );
                }
                else if( copy_length == 3 )
                {
                    conv_func( src_ptr,                dst_ptr );
                    conv_func( src_ptr+  src_chstride, dst_ptr+  dst_chstride );
                    conv_func( src_ptr+2*src_chstride, dst_ptr+2*dst_chstride );
                }
                else if( copy_length == 2 )
                {
                    conv_func( src_ptr,              dst_ptr );
                    conv_func( src_ptr+src_chstride, dst_ptr+dst_chstride );
                }
                else if( copy_length == 1 )
                {
                    conv_func( src_ptr, dst_ptr );
                }
                else
                {
                    for( int32_t ch=0; ch < copy_length; ++ch )
                    {
                        conv_func( src_ptr+ch*src_chstride, dst_ptr+ch*dst_chstride );
                    }
                }

                // Handle the special pixel format conversions
                if( triplicate )
                {
                    // Duplicate the input channel twice more
                    conv_func( src_ptr, dst_ptr+  dst_chstride );
                    conv_func( src_ptr, dst_ptr+2*dst_chstride );
                }
                else if( average )
                {
                    for( int ch=0; ch<3; ++ch )
                    {
                        conv_func( src_ptr+ch*src_chstride, dst_buf.get()+ch*dst_chstride );
                    }
                    avg_func( dst_buf.get(), dst_ptr, 3 );
                }
                if( copy_alpha )
                {
                    conv_func( src_ptr+(src_channels-1)*src_chstride, dst_ptr+(dst_channels-1)*dst_chstride );
                }
                else if( add_alpha )
                {
                    max_func( dst_ptr+(dst_channels-1)*dst_chstride );
                }

                // Finally, adjust destination premultiplication if needed
                if( premultiply_dst )
                {
                    premultiply_dst_func( dst_ptr, dst_ptr, dst_channels );
                }
                // Advance pointers by the appropriate size in bytes
                src_ptr_c += src.cstride();
                dst_ptr_c += dst.cstride();

            } // End of for-each column iteration

            src_ptr_r += src.rstride();
            dst_ptr_r += dst.rstride();

        } // End of for-each row iteration

        src_ptr_p += src.pstride();
        dst_ptr_p += dst.pstride();

    } // End of for-each plane iteration

    return outcome::ok();
} // End function convert

} // End of tmns::image namespace