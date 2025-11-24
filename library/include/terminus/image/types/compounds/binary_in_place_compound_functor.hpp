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
 * @file    Binary_In_Place_Compound_Functor.hpp
 * @author  Marvin Smith
 * @date    8/23/2023
*/
#pragma once

// C++ Libraries
#include <type_traits>

namespace tmns::image::cmp {

template <typename FunctorT,
          typename FuncArg1T,
          typename FuncArg2T>
class Binary_In_Place_Compound_Functor
{
    public:
    
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
                          Argument2T>::apply( func, arg1, arg2 );
        }
    
    private:

        // The general multi-channel case
        template <bool     CompoundB,
                  size_t   ChannelsN,
                  typename Argument1T,
                  typename Argument2T>
        struct Helper
        {
            static Argument1T& apply( const FunctorT&    func,
                                      Argument1T&        arg1,
                                      const Argument2T&  arg2 )
            {
                for( size_t i=0; i<ChannelsN; ++i )
                {
                    func(arg1[i],arg2[i]);
                }
                return arg1;
            }
        };

        // Specialization for non-compound types
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<false,1,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&    func,
                                      Argument1T&        arg1,
                                      const Argument2T&  arg2 )
            {
                func( arg1, arg2 );
                return arg1;
            }
        };

        // Specialization for one-channel types
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,1,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&    func,
                                      Argument1T&        arg1,
                                      const Argument2T&  arg2 )
            {
                func( arg1[0], arg2[0] );
                return arg1;
            }
        };

        // Specialization for two-channel types
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,2,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&    func,
                                      Argument1T&        arg1,
                                      const Argument2T&  arg2 )
            {
                func( arg1[0], arg2[0] );
                func( arg1[1], arg2[1] );
                return arg1;
            }
    };

        // Specialization for three-channel types
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,3,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&    func,
                                      Argument1T&        arg1,
                                      const Argument2T&  arg2 )
            {
                func( arg1[0], arg2[0] );
                func( arg1[1], arg2[1] );
                func( arg1[2], arg2[2] );
                return arg1;
            }
        };

        // Specialization for four-channel types
        template <typename Argument1T,
                  typename Argument2T>
        struct Helper<true,4,Argument1T,Argument2T>
        {
            static Argument1T& apply( const FunctorT&    func,
                                      Argument1T&        arg1,
                                      const Argument2T&  arg2 )
            {
                func( arg1[0], arg2[0] );
                func( arg1[1], arg2[1] );
                func( arg1[2], arg2[2] );
                func( arg1[3], arg2[3] );
                return arg1;
            }
        };

        FunctorT func;

}; // End of Binary_In_Place_Compound_Functor class

template <typename FunctorT,
          typename Argument1T,
          typename Argument2T>
Argument1T& compound_apply_in_place( const FunctorT&   func,
                                     Argument1T&       arg1,
                                     const Argument2T& arg2 )
{
    return Binary_In_Place_Compound_Functor<FunctorT,Argument1T,Argument2T>(func)( arg1, arg2 );
}

} // End of tmns::image::cmp namespace