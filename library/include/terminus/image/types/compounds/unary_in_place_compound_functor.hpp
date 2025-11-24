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
 * @file    Unary_In_Place_Compound_Functor.hpp
 * @author  Marvin Smith
 * @date    8/23/2023
*/
#pragma once

// C++ Libraries
#include <type_traits>

namespace tmns::image::cmp {

template <typename FunctorT,
          typename FuncArgT>
class Unary_In_Place_Compound_Functor
{
    public:

        typedef typename std::add_lvalue_reference<FunctorT>::type func_ref;
    
        Unary_In_Place_Compound_Functor() = default;

        Unary_In_Place_Compound_Functor( func_ref func ) : func(func) {}

        template <class ArgsT>
        struct result {};

        /// FIXME: This seems not to respect the constness of ArgT?  Weird?
        template <typename F,
                  typename ArgumentT>
        struct result<F(ArgumentT)>
        {
            typedef ArgumentT& type;
        };

        template <typename ArgumentT>
        ArgumentT& operator()( ArgumentT& arg ) const
        {
            return Helper<math::Is_Compound<ArgumentT>::value,
                          math::Compound_Channel_Count<ArgumentT>::value,
                          ArgumentT>::apply( func, arg );
        }

        template <typename ArgumentT>
        const ArgumentT& operator()( const ArgumentT& arg ) const
        {
            return Helper<math::Is_Compound<ArgumentT>::value,
                          math::Compound_Channel_Count<ArgumentT>::value,
                          const ArgumentT>::apply(func,arg);
        }

    private:

        // The general multi-channel case
        template <bool     CompoundB,
                  size_t   ChannelsN,
                  typename ArgumentT>
        struct Helper
        {
            static ArgumentT& apply( func_ref   func,
                                     ArgumentT& arg )
            {
                for( size_t i=0; i<ChannelsN; ++i )
                {
                    func(arg[i]);
                }
                return arg;
            }
        };

        // Specialization for non-compound types
        template <typename ArgumentT>
        struct Helper<false,1,ArgumentT>
        {
            static ArgumentT& apply( func_ref   func,
                                     ArgumentT& arg )
            {
                func(arg);
                return arg;
            }
        };

        // Specialization for single-channel types
        template <typename ArgumentT>
        struct Helper<true,1,ArgumentT>
        {
            static ArgumentT& apply( func_ref   func,
                                     ArgumentT& arg )
            {
                func( arg[0] );
                return arg;
            }
        };

        // Specialization for two-channel types
        template <typename ArgumentT>
        struct Helper<true,2,ArgumentT> {
            static ArgumentT& apply( func_ref   func,
                                     ArgumentT& arg )
            {
                func( arg[0] );
                func( arg[1] );
                return arg;
            }
        };

        // Specialization for three-channel types
        template <typename ArgumentT>
        struct Helper<true,3,ArgumentT>
        {
            static ArgumentT& apply( func_ref   func,
                                     ArgumentT& arg )
            {
                func( arg[0] );
                func( arg[1] );
                func( arg[2] );
                return arg;
            }
        };

        // Specialization for four-channel types
        template <typename ArgumentT>
        struct Helper<true,4,ArgumentT>
        {
            static ArgumentT& apply( func_ref   func,
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

}; // End of Unary_In_Place_Compound_Functor

} // End of tmns::image::cmp