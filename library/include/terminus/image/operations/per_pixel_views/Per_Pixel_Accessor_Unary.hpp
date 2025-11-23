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
 * @file    Per_Pixel_Accessor_Unary.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

namespace tmns::image::ops {

/**
 * Pixel accessor for operating on Unary Image-Views
*/
template <typename ImageIterT,
          typename FunctorT>
class Per_Pixel_Accessor_Unary
{
    public:

        /// @brief Result of pixel accessing
        typedef typename std::invoke_result<FunctorT,typename ImageIterT::pixel_type>::type      result_type;

        /// @brief Pixel Type
        typedef typename std::remove_cv<typename std::remove_reference<result_type>::type>::type  pixel_type;

        /// @brief Offset Type (Allows floating-point access if parent does too)
        typedef typename ImageIterT::offset_type                                                  offset_type;

        /**
         * Constructor
         */
        Per_Pixel_Accessor_Unary( const ImageIterT&  iter,
                                  const FunctorT&    func )
          : m_iter( iter ),
            m_func( func )
        {
        }

        /**
         * Get the next column
         */
        Per_Pixel_Accessor_Unary& next_col() { m_iter.next_col(); return (*this); }

        /**
         * Get the previous column
         */
        Per_Pixel_Accessor_Unary& prev_col() { m_iter.prev_col(); return (*this); }

        /**
         * Get the next row
         */
        Per_Pixel_Accessor_Unary& next_row() { m_iter.next_row(); return (*this); }

        /**
         * Get the previous row
         */
        Per_Pixel_Accessor_Unary& prev_row() { m_iter.prev_row(); return (*this); }

        /**
         * Get the next plane
         */
        Per_Pixel_Accessor_Unary& next_plane() { m_iter.next_plane(); return *this; }

        /**
         * Get the previous plane
         */
        Per_Pixel_Accessor_Unary& prev_plane() { m_iter.prev_plane(); return *this; }

        /**
         * Advance the iterator
         */
        Per_Pixel_Accessor_Unary& advance( offset_type di,
                                           offset_type dj,
                                           ssize_t     dp = 0 )
        {
            m_iter.advance(di,dj,dp);
            return *this;
        }

        /**
         * Dereference Operator
        */
        result_type operator*() const
        {
            return m_func(*m_iter);
        }

    private:

        /// @brief  Image Iterator
        ImageIterT      m_iter;

        /// @brief Function to apply
        const FunctorT& m_func;

}; // End of Per_Pixel_Accessor_Unary class

} // End of tmns::image::ops namespace