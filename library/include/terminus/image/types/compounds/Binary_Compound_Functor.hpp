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
 * @file    Binary_Compound_Functor.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// C++ Libraries
#include <type_traits>

namespace tmns::image::cmp {

/**
 *  Takes a pair of multi-channel inputs, splits the pixels apart and applies the
 *  functor against each pair of channels individually.
*/
template <typename FunctorT,
          typename FunctorArg1T,
          typename FunctorArg2T>
class Binary_Compound_Functor
{
    public:

        /**
         * Constructor
        */
        Binary_Compound_Functor() = default;

        /**
         * Constructor given pre-initialized functor
        */
        Binary_Compound_Functor( const FunctorT& func )
          : m_func( func ) {}

        /**
         * Overloadable Result Structure
        */
        template <class ArgsT>
        struct result {};

        /**
         * Pass-by-Value Specialization
        */
        template <typename F,
                  typename Argument1T,
                  typename Argument2T>
        struct result<F(Argument1T,Argument2T)>
        {
            typedef typename math::Compound_Channel_Type<Argument1T>::type               arg1_type;
            typedef typename math::Compound_Channel_Type<Argument2T>::type               arg2_type;
            typedef typename std::invoke_result_t<FunctorT, arg1_type, arg2_type>        result_type;
            typedef typename math::Compound_Channel_Cast<Argument1T,result_type>::type   type;
        };

        /**
         * Call Operator
         */
        template <typename Argument1T,
                  typename Argument2T>
        typename result<Binary_Compound_Functor( Argument1T, Argument2T )>::type
            operator()( const Argument1T& arg1,
                        const Argument2T& arg2 ) const
        {
            typedef typename result<Binary_Compound_Functor(Argument1T,Argument2T)>::type result_type;
            return Helper<math::Is_Compound<result_type>::value,
                          math::Compound_Channel_Count<result_type>::value,
                          result_type,
                          Argument1T,
                          Argument2T>::construct( m_func, arg1, arg2 );
        }

    private:

        /**
         * The general multi-channel case
         *
         * Not recommended as it is not very efficient
         */
        template <bool     CompoundB,
                  size_t   ChannelsN,
                  typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper
        {
            static ResultT construct( const FunctorT&    func,
                                      const Argument1T&  arg1,
                                      const Argument2T&  arg2 )
            {
                ResultT result;
                for( size_t i=0; i<ChannelsN; ++i ){
                    result[i] = func(arg1[i],arg2[i]);
                }
                return result;
            }
        };

        /**
         * Specialization for non-compound types
         */
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<false,1,ResultT,Argument1T,Argument2T>
        {
            static ResultT construct( const FunctorT&    func,
                                      const Argument1T&  arg1,
                                      const Argument2T&  arg2 )
            {
                return func(arg1,arg2);
            }
        };

        /**
         * Specialization for one-channel types
         */
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,1,ResultT,Argument1T,Argument2T>
        {
            static ResultT construct( const FunctorT&   func,
                                      const Argument1T& arg1,
                                      const Argument2T& arg2 )
            {
                return ResultT( func( arg1[0], arg2[0] ) );
            }
        };

        /**
         * Helper struct for two-channel types
         */
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,2,ResultT,Argument1T,Argument2T>
        {
            static ResultT construct( const FunctorT&   func,
                                      const Argument1T& arg1,
                                      const Argument2T& arg2 )
            {
                return ResultT( func( arg1[0], arg2[0] ),
                                func( arg1[1], arg2[1] ) );
            }
        };

        /**
         * Helper struct for 3-channel pixel types
         */
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,3,ResultT,Argument1T,Argument2T>
        {
            static ResultT construct( const FunctorT&    func,
                                      const Argument1T&  arg1,
                                      const Argument2T&  arg2 )
            {
                return ResultT( func( arg1[0], arg2[0] ),
                                func( arg1[1], arg2[1] ),
                                func( arg1[2], arg2[2] ) );
            }
        };

        /**
         * Helper Struct for 4 channel Pixel-Types
        */
        template <typename ResultT,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper<true,4,ResultT,Argument1T,Argument2T>
        {
            /**
             * Construct and bind a result-type to return with
            */
            static ResultT construct( const FunctorT&    func,
                                      const Argument1T&  arg1,
                                      const Argument2T&  arg2 )
            {
                return ResultT( func( arg1[0], arg2[0] ),
                                func( arg1[1], arg2[1] ),
                                func( arg1[2], arg2[2] ),
                                func( arg1[3], arg2[3] ) );
            }
        };

        /// Internal Functor
        FunctorT m_func;

}; // End of Binary_Compound_Functor Class

} // End of tmns::image::cmp namespace