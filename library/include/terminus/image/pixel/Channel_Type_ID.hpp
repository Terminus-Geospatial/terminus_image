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
 * @file    Channel_Type_ID.hpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#pragma once

// Terminus Libraries
#include "Channel_Type_Enum.hpp"

namespace tmns::image {

/**
 * Simple structure for mapping channel-types to Channel_Type_Enum objects
*/
template <class ChannelT>
struct Channel_Type_ID
{
    static constexpr Channel_Type_Enum value = Channel_Type_Enum::UNKNOWN;
};

// Unsigned Integer Types
template <> struct Channel_Type_ID<uint8_t>{  static constexpr Channel_Type_Enum value = Channel_Type_Enum::UINT8;  };
template <> struct Channel_Type_ID<uint16_t>{ static constexpr Channel_Type_Enum value = Channel_Type_Enum::UINT16; };
template <> struct Channel_Type_ID<uint32_t>{ static constexpr Channel_Type_Enum value = Channel_Type_Enum::UINT32; };
template <> struct Channel_Type_ID<uint64_t>{ static constexpr Channel_Type_Enum value = Channel_Type_Enum::UINT64; };

// Signed Integer Types
template <> struct Channel_Type_ID<int8_t>{  static constexpr Channel_Type_Enum value = Channel_Type_Enum::INT8;  };
template <> struct Channel_Type_ID<int16_t>{ static constexpr Channel_Type_Enum value = Channel_Type_Enum::INT16;  };
template <> struct Channel_Type_ID<int32_t>{ static constexpr Channel_Type_Enum value = Channel_Type_Enum::INT32;  };
template <> struct Channel_Type_ID<int64_t>{ static constexpr Channel_Type_Enum value = Channel_Type_Enum::INT64;  };

// Floating-Point Types
template <> struct Channel_Type_ID<float>{  static constexpr Channel_Type_Enum value = Channel_Type_Enum::FLOAT32;  };
template <> struct Channel_Type_ID<double>{ static constexpr Channel_Type_Enum value = Channel_Type_Enum::FLOAT64;  };

} // End of tmns::image namespace