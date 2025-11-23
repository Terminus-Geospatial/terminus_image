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
 * @file    Compound_Utilities.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/image/types/compounds/Binary_Compound_Functor.hpp>
#include <terminus/image/types/compounds/Binary_In_Place_Compound_Functor.hpp>
#include <terminus/image/types/compounds/Unary_Compound_Functor.hpp>
#include <terminus/image/types/compounds/Unary_In_Place_Compound_Functor.hpp>
#include <terminus/math/types/Compound_Types.hpp>

namespace tmns::image {

// These functions take a required ResultT template parameter so the caller
// can specify whether to return by value or by reference, and likewise
// whether to accept the first argument by value or reference.  This is all
// rather annoying, and results in four totally incomprehensible versions of
// a function to do one simple thing.  Is there a better way?  This would all
// be somewhat cleaner if we could do the function enabling in the return type
// rather than the second argument, but that still breaks on some not-so-old
// compilers.

/**
 * Default method for selecting a channel when the channel is not a compound and it is a reference.
 * Just returns the whole thing.
*/
template <typename ResultT,
          typename PixelT>
ResultT compound_select_channel( PixelT& pixel,
                                 typename std::enable_if_t<std::conjunction<typename std::negation< typename math::Is_Compound<PixelT>::value >::value,
                                                                            typename std::is_reference<ResultT>::value>::value,
                                                           size_t> /*channel*/ )
{
    return pixel;
}

/**
 * Select a channel when the channel is not a compound and is not a reference.
 * Returns the whole thing
*/
template <typename ResultT,
          typename PixelT>
ResultT compound_select_channel( const PixelT& pixel, 
                                 typename std::enable_if_t<std::conjunction< typename std::negation< typename math::Is_Compound<PixelT>::value >::value,
                                                                             std::negation< typename std::is_reference<ResultT>::value> >::value, 
                                                           size_t> /*channel*/ )
{
    return pixel;
}

/**
 * Select the channel if it's a compound, and is a reference.
 * Returns the specific channel
*/
template <typename ResultT,
          typename PixelT>
ResultT compound_select_channel( PixelT&                                 pixel, 
                                 typename std::enable_if_t<std::conjunction<typename math::Is_Compound<PixelT>::value, 
                                                                            typename std::is_reference<ResultT>::value>::value,
                                                           size_t>::type channel )
{
    return pixel[channel];
}

/**
 * Select the channel if it's a compound and not a reference
*/
template <typename ResultT,
          typename PixelT>
  ResultT compound_select_channel( const PixelT&                           pixel,
                                   typename std::enable_if_t<std::conjunction<typename math::Is_Compound<PixelT>::value, 
                                                                              std::negation<std::is_reference<ResultT>> >::value,
                                                             size_t>::type channel )
{
    return pixel[channel];
}

/**
 * Helper structure for how to store results from binary operations
*/
template <typename FunctorT,
          typename Argument1T,
          typename Argument2T = void>
struct Compound_Result
{
    typedef typename std::invoke_result_t<cmp::Binary_Compound_Functor<FunctorT,
                                                                       Argument1T,
                                                                       Argument2T>,
                                          Argument1T,
                                          Argument2T> type;
};

/**
 * Helper structure for how to store results fron unary operations
*/
template <typename FunctorT,
          typename ArgT>
struct Compound_Result<FunctorT,ArgT,void>
{
    typedef std::invoke_result_t<cmp::Unary_Compound_Functor<FunctorT,ArgT>,ArgT> type;
};

} // End of tmns::image namespace