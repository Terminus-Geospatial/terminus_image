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
 * @file    Pixel_Accessor_Loose.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once


namespace tmns::image {

// Forward-declaration.
template <class ViewT>
struct Is_Floating_Point_Indexable;

/**
 * A "loose" pixel accessor which isn't tied directory to the memory
 * of this image.  This is used for situations where the image doesn't
 * an "iterable" block of data, thus you need to call the function operator
 * directly.
*/
template <class ImageT>
class Pixel_Accessor_Loose
{
    public:

        /// @brief  Indexing Type
        typedef typename std::conditional<Is_Floating_Point_Indexable<ImageT>::value(), double, int>::type offset_type;

        /// @brief Pixel Type
        typedef typename ImageT::pixel_type  pixel_type;

        /// @brief  Result Type (Notice not a reference)
        typedef typename ImageT::result_type result_type;

        /**
         * Simple Constructor given an image
        */
        Pixel_Accessor_Loose( const ImageT& image ) : m_image(image) {}

        /**
         * Constructor given an image and initial offsets
        */
        Pixel_Accessor_Loose( const ImageT&  image,
                              offset_type    c,
                              offset_type    r,
                              size_t         p = 0 )
        : m_image(image), m_c(c), m_r(r), m_p(p) {}

        /**
         * Jump to the next column
         */
        Pixel_Accessor_Loose& next_col  () { ++m_c; return *this; }

        /**
         * Jump to the next column.  Notice it does a copy.
         */
        Pixel_Accessor_Loose next_col_copy () const
        {
            Pixel_Accessor_Loose tmp(*this);
            tmp.next_col();
            return tmp;
        }

        /**
         * Jump to the previous column
         */
        Pixel_Accessor_Loose& prev_col() { --m_c; return *this; }

        /**
         * Jump to the previous column.  Notice it does a copy.
        */
        Pixel_Accessor_Loose prev_col_copy() const
        {
            Pixel_Accessor_Loose tmp(*this);
            tmp.prev_col();
            return tmp;
        }

        /**
         * Jump to the next row
         */
        Pixel_Accessor_Loose& next_row() { ++m_r; return *this; }

        /**
         * Jump to the next row.  Notice it does a copy
        */
        Pixel_Accessor_Loose next_row_copy() const
        {
            Pixel_Accessor_Loose tmp(*this);
            tmp.next_row();
            return tmp;
        }

        /**
         * Jump to the previous row.
        */
        Pixel_Accessor_Loose& prev_row  () { --m_r; return *this; }

        /**
         * Jump to the previous row.  Notice it does a copy.
        */
        Pixel_Accessor_Loose prev_row_copy() const
        {
            Pixel_Accessor_Loose tmp(*this);
            tmp.prev_row();
            return tmp;
        }

        /**
         * Jump to the next plane
        */
        Pixel_Accessor_Loose& next_plane() { ++m_p; return *this; }

        /**
         * Jump to the next plane.  Notice it does a copy.
        */
        Pixel_Accessor_Loose next_plane_copy() const
        {
            Pixel_Accessor_Loose tmp(*this);
            tmp.next_plane();
            return tmp;
        }

        /**
         * Jump to the previous plane
        */
        Pixel_Accessor_Loose& prev_plane() { --m_p; return *this; }

        /**
         * Jump to the previous plane.  Notice it does a copy.
        */
        Pixel_Accessor_Loose prev_plane_copy() const
        {
            Pixel_Accessor_Loose tmp(*this);
            tmp.prev_plane();
            return tmp;
        }

        /**
         * Move the iterator to the specific offset, relative to the current position.
        */
        Pixel_Accessor_Loose& advance( offset_type dc,
                                       offset_type dr,
                                       ssize_t     dp = 0 )
        {
            m_c += dc;
            m_r += dr;
            m_p += (int)dp;
            return *this;
        }

        /**
         * Move the iterator to the specific offset, relative to the current position.
         * Notice this does a copy.
        */
        Pixel_Accessor_Loose advance_copy ( offset_type dc,
                                            offset_type dr,
                                            ssize_t     dp = 0 ) const
        {
            Pixel_Accessor_Loose tmp(*this);
            tmp.advance(dc,dr,dp);
            return tmp;
        }

        /**
         * Dereference the iterator
        */
        result_type operator*() const
        {
            return m_image(m_c,m_r,m_p);
        }

        /**
         * Get this class name
        */
        static std::string class_name()
        {
            return "Pixel_Accessor_Loose";
        }

        static std::string full_name()
        {
            return class_name() + "<" + pixel_type::class_name() + ">";
        }

    private:

        const ImageT& m_image;
        offset_type m_c { 0 };
        offset_type m_r { 0 };
        int m_p { 0 };

}; // End Pixel_Accessor_Loose Class

} // End of tmns::image namespace