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
 * @file    Pixel_Accessor_MemStride.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once


namespace tmns::image {

/**
 * A pixel accessor class that iterates of blocks of memory.
 *
 * This is similar to how OpenCV operates and just does block iteration.
 * Used by `Image_Memory` and other "in-memory" models.
*/
template <typename PixelT>
class Pixel_Accessor_MemStride
{
    public:

        /// @brief  Image Pixel Type
        typedef PixelT pixel_type;

        /// @brief Resulting Image Type (Notice Reference)
        typedef PixelT& result_type;

        /// @brief Offset Type
        typedef ssize_t offset_type;

        /**
         * Constructor
         *
         * @param ptr Pointer to pixel data (Note the pixel-units)
         * @param rstride Row-stride (Distance between each row)
         * @param pstride Plane-stride (Distance between each plane)
        */
        Pixel_Accessor_MemStride( PixelT* ptr,
                                  ssize_t rstride,
                                  ssize_t pstride )
          : m_ptr( ptr ),
            m_origin( ptr ),
            m_rstride( rstride ),
            m_pstride( pstride )
        {
        }

        /**
         * Advance the iterator to the next column
        */
        Pixel_Accessor_MemStride&  next_col() { ++m_ptr; return *this; }

        /**
         * Advance the iterator to the next column
         * Const-capable, resulting in copy of the iterator
         */
        Pixel_Accessor_MemStride next_col_copy() const
        {
            Pixel_Accessor_MemStride tmp(*this);
            tmp.next_col();
            return tmp;
        }

        /**
         * Advance the iterator to the previous column
        */
        Pixel_Accessor_MemStride&  prev_col() { --m_ptr; return *this; }

        /**
         * Advance the iterator to the previous column.
         * Const-capable, resulting in copy of the iterator
         */
        Pixel_Accessor_MemStride prev_col_copy() const
        {
            Pixel_Accessor_MemStride tmp(*this);
            tmp.prev_col();
            return tmp;
        }

        /**
         * Advance the iterator to the next row
        */
        Pixel_Accessor_MemStride&  next_row() { m_ptr += m_rstride; return *this; }

        /**
         * Advance the iterator to the next row.
         * Const-capable, resulting in copy of the iterator
         */
        Pixel_Accessor_MemStride next_row_copy() const
        {
            Pixel_Accessor_MemStride tmp(*this);
            tmp.next_row();
            return tmp;
        }

        /**
         * Advance the iterator to the previous row
        */
        Pixel_Accessor_MemStride&  prev_row() { m_ptr -= m_rstride; return *this; }

        /**
         * Advance the iterator to the previous row.
         * Const-capable, resulting in copy of the iterator
         */
        Pixel_Accessor_MemStride prev_row_copy() const
        {
            Pixel_Accessor_MemStride tmp(*this);
            tmp.prev_row();
            return tmp;
        }

        /**
         * Advance the iterator to the next image plane
        */
        Pixel_Accessor_MemStride&  next_plane() { m_ptr += m_pstride; return *this; }

        /**
         * Advance the iterator to the next image plane.
         * Const-capable, resulting in a copy of the iterator.
        */
        Pixel_Accessor_MemStride next_plane_copy() const
        {
            Pixel_Accessor_MemStride tmp(*this);
            tmp.next_plane();
            return tmp;
        }

        /**
         * Advance the iterator to the previous image plane
        */
        Pixel_Accessor_MemStride&  prev_plane() { m_ptr -= m_pstride; return *this; }

        /**
         * Advance the iterator to the previous image plane.
         * Const-capable, resulting in a copy of the iterator.
         */
        Pixel_Accessor_MemStride prev_plane_copy() const
        {
            Pixel_Accessor_MemStride tmp(*this);
            tmp.prev_plane();
            return tmp;
        }

        /**
         * Advance the iterator to the specified position in offsets.
        */
        Pixel_Accessor_MemStride&  advance( ssize_t diff_cols,
                                            ssize_t diff_rows,
                                            ssize_t diff_planes = 0 )
        {
            m_ptr += diff_cols + diff_rows * m_rstride + diff_planes * m_pstride;
            return *this;
        }

        /**
         * Advance the iterator to the specified position in offsets.
         * Const-capable, resulting in a copy of the iterator.
         */
        Pixel_Accessor_MemStride advance_copy ( ssize_t diff_cols,
                                                ssize_t diff_rows,
                                                ssize_t diff_planes = 0 ) const
        {
            Pixel_Accessor_MemStride tmp(*this);
            tmp.advance( diff_cols,
                         diff_rows,
                         diff_planes );
            return tmp;
        }

        /**
         * Operator returns the pixel value at the current location
        */
        result_type operator* () const
        {
            return *m_ptr;
        }

        ssize_t distance() const
        {
            return std::distance( m_origin, m_ptr );
        }

        /**
         * Get this class name
        */
        static std::string class_name()
        {
            return "Pixel_Accessor_MemStride";
        }

        static std::string full_name()
        {
            return class_name() + "<" + pixel_type::class_name() + ">";
        }

    private:

        /// @brief Raw pointer to pixels.  Notice the type isn't a channel or base type.
        PixelT* m_ptr { nullptr };

        /// @brief Position to origin (only for estimating offsets when troubleshooting)
        PixelT* m_origin { nullptr };

        /// @brief  Row-Stride
        ssize_t m_rstride { 0 };

        /// @brief  Plane-Stride
        ssize_t m_pstride { 0 };

}; // End of Pixel_Accessor_MemStride Class

} // end of tmns::image namespace