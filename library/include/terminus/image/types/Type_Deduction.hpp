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
 * @file    Type_Deduction.hpp
 * @author  Marvin Smith
 * @date    8/11/2023
*/
#pragma once

// C++ Libraries
#include <type_traits>

// Boost Libraries
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>

// Terminus Libraries
#include "Compound_Types.hpp"

namespace tmns::image {

// If you have an error message that leads you here, you tried to apply a
// functor to two user-defined (non-arithmetic) types, and we didn't know
// what type to return.
template <typename T>
struct Type_Deduction_Error;

namespace types::detail {

// When you don't know what to do
struct unknown_type_tag : public boost::mpl::int_<10000> {};

#define TMNS_INTERNAL_TYPE_DEDUCTION( Type, Number )        \
    static_assert( std::is_arithmetic<Type>::value );       \
    template <>                                             \
    struct Type_Deduction_Index<Type>                       \
    {                                                       \
      typedef boost::mpl::int_<Number> type;                \
      BOOST_STATIC_CONSTANT(uint32_t, value = type::value); \
    }                                                       \

    /**
     * Assigns a type to an integer value, so we can do comparisons
    */
    template <class T>
    struct Type_Deduction_Index
    {
        typedef unknown_type_tag type;
        BOOST_STATIC_CONSTANT(uint32_t, value = type::value);
    };

    // I just learned that for chars, there is no "default" sign/unsign
    TMNS_INTERNAL_TYPE_DEDUCTION(           char,  100 );
    TMNS_INTERNAL_TYPE_DEDUCTION(    signed char,  200 );
    TMNS_INTERNAL_TYPE_DEDUCTION(  unsigned char,  300 );
    TMNS_INTERNAL_TYPE_DEDUCTION(        int16_t,  400 );
    TMNS_INTERNAL_TYPE_DEDUCTION(       uint16_t,  500 );
    TMNS_INTERNAL_TYPE_DEDUCTION(        int32_t,  600 );
    TMNS_INTERNAL_TYPE_DEDUCTION(       uint32_t,  700 );
    TMNS_INTERNAL_TYPE_DEDUCTION(        int64_t,  800 );
    TMNS_INTERNAL_TYPE_DEDUCTION(       uint64_t,  900 );
    TMNS_INTERNAL_TYPE_DEDUCTION(          float, 1000 );
    TMNS_INTERNAL_TYPE_DEDUCTION(         double, 1100 );
    TMNS_INTERNAL_TYPE_DEDUCTION(    long double, 1200 );

} // end of types::detail namespace

/**
 * Base class for deducing a type, given 2 inputs
*/
template <typename T1,
          typename T2>
struct Type_Deduction_Helper
{
    typedef typename types::detail::Type_Deduction_Index<T1>::type I1;
    typedef typename types::detail::Type_Deduction_Index<T2>::type I2;

    // If an error message leads you here, you tried to apply a functor to two
    // user-defined (non-arithmetic) types, and we didn't know what type to
    // return. (We catch the case where the types are actually the same in the
    // next specialization.)
    static_assert( !std::is_same<I1, I2>::value );

    // If an error message leads you here, you have an arithmetic type that we
    // haven't identified. Please report this as a bug.
    //
    // (Implementation note: TMNS_INTERNAL_TYPE_DEDUCTION contains an
    // is_arithmetic check which makes this logic correct)
    static_assert( !( std::conjunction< std::is_arithmetic<T1>, std::is_same<I1, types::detail::unknown_type_tag> >::value ) );
    static_assert( !( std::conjunction< std::is_arithmetic<T2>, std::is_same<I2, types::detail::unknown_type_tag> >::value ) );

    typedef typename boost::mpl::eval_if< boost::mpl::greater<I1, I2>,
                                          boost::mpl::identity<T1>,
                                          boost::mpl::identity<T2> >::type type;
}; // End Type_Deduction_Helper class


/**
 * When both types are of the same class
*/
template <typename ArgumentT>
struct Type_Deduction_Helper<ArgumentT, ArgumentT>
{
    typedef ArgumentT type;
}; // End of Type_Deduction_Helper struct

// ********************************************************************
// Now we set up the default promotion behavior for general types.
// This includes promotions of compound types (e.g. pixel types)
// when used in operations with fundamental types.  Thus for example
// multiplying PixelRGB<int> by float should return PixelRGB<float>.
// We address this by recursing on the template parameter.  Users
// can explicitly specialize PromoteTypeSpecialization<> to support
// custom behaviors when both arguments are custom types.
// ********************************************************************

// Forward declaration (See after the helpers)
template <typename Argument1T,
          typename Argument2T>
class Promote_Type_Specialization;


// This helper class forwards the default case to TypeDeductionHelper.
template <typename Argument1T,
          typename Argument2T,
          bool Arg1IsCompound,
          bool Arg2IsCompound>
struct Promote_Type_Specialization_Helper : public Type_Deduction_Helper<Argument1T,Argument2T> {};

/**
 * When Argument2 is a compound type
*/
template <typename Argument1T,
          typename Argument2T>
struct Promote_Type_Specialization_Helper<Argument1T,Argument2T,true,false>
{
    typedef typename Compound_Channel_Cast<Argument1T,
                                           typename Promote_Type_Specialization<typename Compound_Channel_Type<Argument1T>::type, Argument2T>::type>::type type;
}; // End of Promote_Type_Specialization_Helper struct

/**
 * When Argument2 is a compound type
*/
template <typename Argument1T,
          typename Argument2T>
struct Promote_Type_Specialization_Helper<Argument1T,Argument2T,false,true>
{
    typedef typename Compound_Channel_Cast<Argument2T,
                                           typename Promote_Type_Specialization<Argument1T,
                                                                                typename Compound_Channel_Type<Argument2T>::type>::type>::type type;
}; // End of Promote_Type_Specialization_Helper struct

// Forward declaration for 2nd helper
template <typename Argument1T,
          typename Argument2T,
          bool Same_Wrapper>
struct Promote_Type_Specialization_Helper_2;

/**
 * When the types are the same
*/
template <typename Argument1T,
          typename Argument2T>
struct Promote_Type_Specialization_Helper_2<Argument1T, Argument2T, true>
{
    typedef typename Compound_Channel_Cast<Argument1T,
                                           typename Promote_Type_Specialization<typename Compound_Channel_Type<Argument1T>::type, 
                                                                                typename Compound_Channel_Type<Argument2T>::type>::type>::type type;
}; // End of Promote_Type_Specialization_Helper_2 struct

/**
 * When the types are not the same
*/
template <typename Argument1T,
          typename Argument2T>
struct Promote_Type_Specialization_Helper_2<Argument1T, Argument2T, false>
{
    typedef typename Type_Deduction_Helper<Argument1T,Argument2T>::type type;
}; // Promote_Type_Specialization_Helper_2

/**
 * Recursive behavior when both arguments are compound types
 */
template <typename Argument1T,
          typename Argument2T>
struct Promote_Type_Specialization_Helper<Argument1T,
                                          Argument2T,
                                          true,
                                          true>
{
    typedef typename Promote_Type_Specialization_Helper_2<Argument1T,
                                                          Argument2T,
                                                          std::is_same<Argument1T,
                                                                       typename Compound_Channel_Cast<Argument2T,
                                                                                                      typename Compound_Channel_Type<Argument1T>::type>::type>::value>::type type;
}; // End of Promote_Type_Specialization_Helper struct

/**
 * Dispatch to the appropriate helper based on which if any
 * argument is a compound type.
*/
template <typename Argument1T,
          typename Argument2T>
class Promote_Type_Specialization : public Promote_Type_Specialization_Helper<Argument1T,
                                                                              Argument2T,
                                                                              Is_Compound<Argument1T>::value,
                                                                              Is_Compound<Argument2T>::value>
{}; // End of Promote_Type_Specialization struct

// ********************************************************************
// Finally, we provide forwarding classes for the different operations
// that the user can specifically specialize in special cases if
// needed.  The classes the user *uses*, such as SumType, simply
// strip off any CV-qualification and forward to the appropriate
// specialization type.  If the user needs to override the type
// deduction behavior, they should generally do so by overriding one
// of the ...Specialization classes, so that they don't have to worry
// about CV-qualification themselves.
// ********************************************************************

/**
 * Type for performing Summation with
*/
template <typename Argument1T,
          typename Argument2T>
struct Sum_Type_Specialization : public Promote_Type_Specialization<Argument1T,Argument2T>{};

/**
 * Type for performing Difference with
*/
template <typename Argument1T, 
          typename Argument2T>
struct Difference_Type_Specialization : public Promote_Type_Specialization<Argument1T,Argument2T> {};

/**
 * Type for performing multiplication
*/
template <typename Argument1T, 
          typename Argument2T>
struct Product_Type_Specialization : public Promote_Type_Specialization<Argument1T,Argument2T> {};

/**
 * Type for performing division
*/
template <typename Argument1T, 
          typename Argument2T>
struct Quotient_Type_Specialization : public Promote_Type_Specialization<Argument1T,Argument2T> {};

/**
 * Promotion specialization which removes the const component
*/
template <typename Argument1T, 
          typename Argument2T>
struct Promote_Type
{
    typedef typename Promote_Type_Specialization<typename std::remove_cv<Argument1T>::type,
                                                 typename std::remove_cv<Argument2T>::type>::type type;
}; // End of Promote_Type struct

/**
 * Summation type which removes const
*/
template <typename Argument1T, 
          typename Argument2T>
struct Sum_Type
{
    typedef typename Sum_Type_Specialization<typename std::remove_cv<Argument1T>::type,
                                             typename std::remove_cv<Argument2T>::type>::type type;
}; // End of Sum_Type struct

/**
 * Difference type which removes const
*/
template <typename Argument1T, 
          typename Argument2T>
struct Difference_Type
{
    typedef typename Difference_Type_Specialization<typename std::remove_cv<Argument1T>::type,
                                                    typename std::remove_cv<Argument2T>::type>::type type;
}; // End of Difference_Type struct

/**
 * Product type which removes const
*/
template <typename Argument1T, 
          typename Argument2T>
struct Product_Type
{
    typedef typename Product_Type_Specialization<typename std::remove_cv<Argument1T>::type,
                                                 typename std::remove_cv<Argument2T>::type>::type type;
}; // End of Product_Type struct

/**
 * Division type which strips the const
*/
template <typename Argument1T, 
          typename Argument2T>
struct Quotient_Type
{
    typedef typename Quotient_Type_Specialization<typename std::remove_cv<Argument1T>::type,
                                                  typename std::remove_cv<Argument2T>::type>::type type;
}; // End of Quotient_Type struct

} // End of tmns::image namespace