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
 * @file    Pixel_Base.hpp
 * @author  Marvin Smith
 * @date    7/11/2023
*/
#pragma once

// Terminus Image Libraries
#include <terminus/image/pixel/Channel_Cast_Utilities.hpp>
#include <terminus/image/types/Compound_Utilities.hpp>
#include <terminus/math/types/Functors.hpp>

namespace tmns::image {

template <typename DerivedT>
class Pixel_Base
{
    public:

        /// Methods to access derived type
        DerivedT      & impl()       { return static_cast<DerivedT      &>(*this); }
        DerivedT const& impl() const { return static_cast<DerivedT const&>(*this); }

}; // End of PixelBase Class

/**
 * Unary Negation Operator of a Pixel
*/
template <typename PixelT>
typename Compound_Result<math::Arg_Negation_Functor,PixelT>::type          \
  operator - ( const Pixel_Base<PixelT>& pixel )
{
    return pix::compound_apply( math::Arg_Negation_Functor(),
                                pixel.impl() );
}

/**
 * Addition Operator between a Pixel and a Pixel
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t<math::Compound_Is_Compatible<Pixel1T,Pixel2T>::value, 
                          Compound_Result<math::Arg_Arg_Sum_Functor,
                                          Pixel1T,
                                          Pixel2T> >::type
   operator + ( const Pixel_Base<Pixel1T>& pixel1, 
                const Pixel_Base<Pixel2T>& pixel2 )
{
    return pix::compound_apply( math::Arg_Arg_Sum_Functor(),
                                pixel1.impl(),
                                pixel2.impl() );
}

/**
 * Addition between a pixel and a scalar
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value,
                           typename Compound_Result<math::Arg_Val_Sum_Functor<ScalarT>,
                           PixelT>::type >
  operator + ( const Pixel_Base<PixelT>& pixel,
               ScalarT                   scalar )
{
    return pix::compound_apply( math::Arg_Val_Sum_Functor<ScalarT>(scalar),
                                pixel.impl() );
}

/**
 * Addition between a scalar and a pixel
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value,
                           typename Compound_Result<math::Val_Arg_Sum_Functor<ScalarT>,
                           PixelT>::type >::type
    operator + ( ScalarT                   scalar,
                 const Pixel_Base<PixelT>& pixel )
{
    return pix::compound_apply( math::Val_Arg_Sum_Functor<ScalarT>(scalar),
                                pixel.impl() );
}

/**
 * In-Place Addition betweeen 2 pixels
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t< math::Compound_Is_Compatible<Pixel1T,Pixel2T>::value, Pixel1T&>
    operator += ( Pixel_Base<Pixel1T>&       pixel1,
                  const Pixel_Base<Pixel2T>& pixel2 )
{
    return cmp::compound_apply_in_place( math::Arg_Arg_In_Place_Sum_Functor(),
                                         pixel1.impl(),
                                         pixel2.impl() );
}

/**
 * In-Place Addition between Pixel and Scalar
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value, PixelT&>::type
  operator += ( const Pixel_Base<PixelT>& pixel,
                ScalarT                   scalar )
{
    return pix::compound_apply_in_place( math::Arg_Val_In_Place_Sum_Functor<ScalarT>( scalar),
                                         pixel.impl() );
}

/**
 * Difference Operator between a Pixel and a Pixel
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t<math::Compound_Is_Compatible<Pixel1T,Pixel2T>::value, 
                          typename Compound_Result<math::Arg_Arg_Difference_Functor,
                                                   Pixel1T,
                                                   Pixel2T>::type >::type
   operator - ( const Pixel_Base<Pixel1T>& pixel1, 
                const Pixel_Base<Pixel2T>& pixel2 )
{
    return pix::compound_apply( math::Arg_Arg_Difference_Functor(),
                                pixel1.impl(),
                                pixel2.impl() );
}

/**
 * Difference between a pixel and a scalar
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value,
                           typename Compound_Result<math::Arg_Val_Difference_Functor<ScalarT>,
                           PixelT>::type >
  operator - ( const Pixel_Base<PixelT>& pixel,
               ScalarT                   scalar )
{
    return pix::compound_apply( math::Arg_Val_Difference_Functor<ScalarT>(scalar),
                                pixel.impl() );
}

/**
 * Difference between a scalar and a pixel
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value,
                           typename Compound_Result<math::Val_Arg_Difference_Functor<ScalarT>,
                           PixelT>::type >::type
    operator - ( ScalarT                   scalar,
                 const Pixel_Base<PixelT>& pixel )
{
    return pix::compound_apply( math::Val_Arg_Difference_Functor<ScalarT>(scalar),
                                pixel.impl() );
}

/**
 * In-Place Difference betweeen 2 pixels
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t< math::Compound_Is_Compatible<Pixel1T,Pixel2T>::value, Pixel1T&>::type
    operator -= ( const Pixel_Base<Pixel1T>& pixel1,
                  const Pixel_Base<Pixel2T>& pixel2 )
{
    return pix::compound_apply_in_place( math::Arg_Arg_In_Place_Difference_Functor(),
                                         pixel1.impl(),
                                         pixel2.impl() );
}

/**
 * In-Place Difference between Pixel and Scalar
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value, PixelT&>::type
  operator -= ( const Pixel_Base<PixelT>& pixel,
                ScalarT                   scalar )
{
    return pix::compound_apply_in_place( math::Arg_Val_In_Place_Difference_Functor<ScalarT>( scalar),
                                         pixel.impl() );
}

/**
 * Multiplication Between 2 Pixels
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t< math::Compound_Is_Compatible<Pixel1T,Pixel2T>::value,
                           typename Compound_Result<math::Arg_Arg_Product_Functor,
                                                    Pixel1T,
                                                    Pixel2T>::type >
  operator * ( const Pixel_Base<Pixel1T>& pixel1,
               const Pixel_Base<Pixel2T>& pixel2 )
{
    return pix::compound_apply( math::Arg_Arg_Product_Functor(),
                                pixel1.impl(),
                                pixel2.impl() );
}

/**
 * Product between Pixel and Scalar
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value,
                           typename Compound_Result<math::Arg_Val_Product_Functor<ScalarT>,
                           PixelT>::type >
  operator * ( const Pixel_Base<PixelT>& pixel,
               ScalarT                   scalar )
{
    return pix::compound_apply( math::Arg_Val_Product_Functor<ScalarT>(scalar),
                                pixel.impl() );
}

/**
 * Product between Scalar and Pixel
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value,
                           typename Compound_Result<math::Val_Arg_Product_Functor<ScalarT>,
                           PixelT>::type >::type
    operator * ( ScalarT                   scalar,
                 const Pixel_Base<PixelT>& pixel )
{
    return pix::compound_apply( math::Val_Arg_Product_Functor<ScalarT>(scalar),
                                pixel.impl() );
}

/**
 * In-Place multiplication between 2 pixels
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t< math::Compound_Is_Compatible<Pixel1T,Pixel2T>::value, Pixel1T&>::type
    operator *= ( const Pixel_Base<Pixel1T>& pixel1,
                  const Pixel_Base<Pixel2T>& pixel2 )
{
    return pix::compound_apply_in_place( math::Arg_Arg_In_Place_Product_Functor(),
                                         pixel1.impl(),
                                         pixel2.impl() );
}

/**
 * In-Place multiplication between Pixel and a Scalar
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value, PixelT&>::type
  operator *= ( const Pixel_Base<PixelT>& pixel,
                ScalarT                   scalar )
{
    return pix::compound_apply_in_place( math::Arg_Val_In_Place_Product_Functor<ScalarT>( scalar ),
                                         pixel.impl() );
}

/**
 * Division Between 2 pixels
*/

/**
 * Division Between Pixel and a Scalar
*/
template <typename PixelT,
          typename ScalarT>
typename std::enable_if_t< math::Is_Scalar<ScalarT>::value,
                           typename Compound_Result<math::Arg_Val_Quotient_Functor<ScalarT>,
                           PixelT>::type >
  operator / ( const Pixel_Base<PixelT>& pixel,
               ScalarT                   scalar )
{
    return pix::compound_apply( math::Arg_Val_Quotient_Functor<ScalarT>(scalar),
                                pixel.impl() );
}

/**
 * Division Between Scalar and a Pixel
*/

/**
 * In-Place Division between 2 pixels
*/

/**
 * In-Place Division between Pixel and a Scalar
*/


/**
 * Pixel Equality
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t< math::Compound_Is_Compatible<Pixel1T,Pixel2T>::value,
                           bool >::type
   operator == ( const Pixel_Base<Pixel1T>& pixel1,
                 const Pixel_Base<Pixel2T>& pixel2 )
{
    for( size_t c = 0; c < math::Compound_Channel_Count<Pixel1T>::value; ++c )
        if( compound_select_channel<typename math::Compound_Channel_Type<Pixel1T>::type>(pixel1.impl(),c) !=
            compound_select_channel<typename math::Compound_Channel_Type<Pixel2T>::type>(pixel2.impl(),c) )
            return false;
    return true;
}

/**
 * Pixel Inequality
*/
template <typename Pixel1T,
          typename Pixel2T>
typename std::enable_if_t< math::Compound_Is_Compatible<Pixel1T,Pixel2T>::ptr_t, bool >
   operator != ( const Pixel_Base<Pixel1T>& pixel1,
                 const Pixel_Base<Pixel2T>& pixel2 )
{
    return !( pixel1 == pixel2 );
}

// @todo
/**
 * // *******************************************************************
  // Default mathematical function overlaods
  // *******************************************************************

  VW_PIXEL_MATH_STD_UNARY_FUNCTION(acos, vw::math::ArgAcosFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(asin, vw::math::ArgAsinFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(atan, vw::math::ArgAtanFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(cos, vw::math::ArgCosFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(sin, vw::math::ArgSinFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(tan, vw::math::ArgTanFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(cosh, vw::math::ArgCoshFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(sinh, vw::math::ArgSinhFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(tanh, vw::math::ArgTanhFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(exp, vw::math::ArgExpFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(log, vw::math::ArgLogFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(log10, vw::math::ArgLog10Functor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(sqrt, vw::math::ArgSqrtFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(ceil, vw::math::ArgCeilFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(floor, vw::math::ArgFloorFunctor)

  VW_PIXEL_MATH_STD_BINARY_PP_FUNCTION(atan2, vw::math::ArgArgAtan2Functor)
  VW_PIXEL_MATH_BINARY_PS_FUNCTION(atan2, vw::math::ArgValAtan2Functor)
  VW_PIXEL_MATH_BINARY_SP_FUNCTION(atan2, vw::math::ValArgAtan2Functor)
  VW_PIXEL_MATH_STD_BINARY_PP_FUNCTION(pow, vw::math::ArgArgPowFunctor)
  VW_PIXEL_MATH_BINARY_PS_FUNCTION(pow, vw::math::ArgValPowFunctor)
  VW_PIXEL_MATH_BINARY_SP_FUNCTION(pow, vw::math::ValArgPowFunctor)

  VW_PIXEL_MATH_STD_BINARY_PP_FUNCTION(hypot, vw::math::ArgArgHypotFunctor)
  VW_PIXEL_MATH_BINARY_PS_FUNCTION(hypot, vw::math::ArgValHypotFunctor)
  VW_PIXEL_MATH_BINARY_SP_FUNCTION(hypot, vw::math::ValArgHypotFunctor)

#ifndef WIN32
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(acosh, vw::math::ArgAcoshFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(asinh, vw::math::ArgAsinhFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(atanh, vw::math::ArgAtanhFunctor)
#ifdef VW_HAVE_EXP2
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(exp2, vw::math::ArgExp2Functor)
#endif
#ifdef VW_HAVE_LOG2
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(log2, vw::math::ArgLog2Functor)
#endif
#ifdef VW_HAVE_TGAMMA
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(tgamma,vw::math::ArgTgammaFunctor)
#endif
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(expm1, vw::math::ArgExpm1Functor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(log1p, vw::math::ArgLog1pFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(cbrt, vw::math::ArgCbrtFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(erf, vw::math::ArgErfFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(erfc, vw::math::ArgErfcFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(lgamma,vw::math::ArgLgammaFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(round, vw::math::ArgRoundFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(trunc, vw::math::ArgTruncFunctor)

  VW_PIXEL_MATH_STD_BINARY_PP_FUNCTION(copysign, vw::math::ArgArgCopysignFunctor)
  VW_PIXEL_MATH_BINARY_PS_FUNCTION(copysign, vw::math::ArgValCopysignFunctor)
  VW_PIXEL_MATH_BINARY_SP_FUNCTION(copysign, vw::math::ValArgCopysignFunctor)
  VW_PIXEL_MATH_STD_BINARY_PP_FUNCTION(fdim, vw::math::ArgArgFdimFunctor)
  VW_PIXEL_MATH_BINARY_PS_FUNCTION(fdim, vw::math::ArgValFdimFunctor)
  VW_PIXEL_MATH_BINARY_SP_FUNCTION(fdim, vw::math::ValArgFdimFunctor)
#endif // WIN32

  VW_PIXEL_MATH_UNARY_FUNCTION(real, vw::math::ArgRealFunctor)
  VW_PIXEL_MATH_UNARY_FUNCTION(imag, vw::math::ArgImagFunctor)
  VW_PIXEL_MATH_STD_UNARY_FUNCTION(abs,  vw::math::ArgAbsFunctor)
  VW_PIXEL_MATH_UNARY_FUNCTION(conj, vw::math::ArgConjFunctor)
  VW_PIXEL_MATH_UNARY_FUNCTION(square, vw::math::ArgSquareFunctor)

*/

} // End of tmns::image namespace