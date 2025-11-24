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
 * @file    Unary_Compound_Functor.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// C++ Libraries
#include <type_traits>

namespace tmns::image::cmp {

/**
 * Class for breaking apart Pixels and applying a functor against
 * each channel individually
*/
template <typename FunctorT,
          typename FuncArgT>
class Unary_Compound_Functor
{
    public:

        /**
         * Default Constructor
        */
        Unary_Compound_Functor() = default;

        /**
         * Parameterized Constructor with pre-initialized functor
        */
        Unary_Compound_Functor( const FunctorT& func )
          : m_func( func ) {}

        /**
         * Overloadable result type
        */
        template <typename ArgsT>
        struct result {};

        /**
         * Standard Result Type
        */
        template <typename F,
                  typename ArgT>
        struct result<F(ArgT)>
        {
            typedef typename math::Compound_Channel_Type<ArgT>::type             arg_type;
            typedef typename std::invoke_result_t<FunctorT,arg_type>             result_type;
            typedef typename math::Compound_Channel_Cast<ArgT,result_type>::type type;
        };

        /**
         * Call Operator
        */
        template <typename ArgT>
        typename result<Unary_Compound_Functor(ArgT)>::type
            operator()( const ArgT& arg ) const
        {
            typedef typename result<Unary_Compound_Functor(ArgT)>::type result_type;
            return Helper<math::Is_Compound<result_type>::value,
                          math::Compound_Channel_Count<result_type>::value,
                          result_type,
                          ArgT>::construct( m_func, arg );
        }

    private:

        /**
         * Helper Structure for general use case
         */
        template <bool     CompoundB,
                  size_t   ChannelsN,
                  typename ResultT,
                  typename ArgT>
        struct Helper
        {
            static ResultT construct( const FunctorT& func,
                                      const ArgT&     arg )
            {
                ResultT result;
                for( size_t i=0; i<ChannelsN; ++i )
                {
                    result[i] = func(arg[i]);
                }
                return result;
            }
        }; // End of Helper Struct

        /**
         * Specialization for non-compound types
         */
        template <typename ResultT,
                  typename ArgT>
        struct Helper<false,1,ResultT,ArgT>
        {
            static ResultT construct( const FunctorT& func,
                                      const ArgT&     arg )
            {
                return func(arg);
            }
        };

        /**
         * Specialization for single-channel types
         */
        template <typename ResultT,
                  typename ArgT>
        struct Helper<true,1,ResultT,ArgT>
        {
            static ResultT construct( const FunctorT& func,
                                      const ArgT&     arg )
            {
                return ResultT( func(arg[0]) );
            }
        };

        /**
         * Specialization for two-channel types
         */
        template <typename ResultT,
                  typename ArgT>
        struct Helper<true,2,ResultT,ArgT>
        {
            static ResultT construct( const FunctorT& func,
                                      const ArgT&     arg )
            {
                return ResultT( func(arg[0]), func(arg[1]) );
            }
        };

        /**
         * Specialization for three-channel types
         */
        template <typename ResultT,
                  typename ArgT>
        struct Helper<true,3,ResultT,ArgT>
        {
            static ResultT construct( const FunctorT& func,
                                      const ArgT&     arg )
            {
                return ResultT( func( arg[0] ),
                                func( arg[1] ),
                                func( arg[2] ) );
            }
        };

        /**
         * Specialization for four-channel types
         */
        template <typename ResultT,
                  typename ArgT>
        struct Helper<true,4,ResultT,ArgT>
        {
            static ResultT construct( const FunctorT& func,
                                      const ArgT&     arg )
            {
                return ResultT( func( arg[0] ),
                                func( arg[1] ),
                                func( arg[2] ),
                                func( arg[3] ) );
            }
        };

        /// Functor to apply
        FunctorT m_func;

}; // End of Unary_Compound_Functor Class

} // End of tmns::image::cmp namespace