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
 * @file    Image.hpp
 * @author  Marvin Smith
 * @date    7/5/2023
*/
#pragma once

// Terminus Image Libraries
#include "../../feature/Interest_Point.hpp"
#include "../metadata/Metadata_Container_Base.hpp"
#include "../pixel/Channel_Type_ID.hpp"
#include "../pixel/Pixel_Format_ID.hpp"
#include "Image_Format.hpp"
#include "Pixel_Iterator.hpp"

// Terminus Libraries
#include <terminus/math/Rectangle.hpp>

namespace tmns::image {

/**
 * @class Image_Base
 *
 * Base image type.  This is important cause we want either lazy or eager
 * models.  This will be the key base class keeping them tied together.  Please be
 * very careful when touching this class.
 *
 * This class uses Curiously-Recurring Template Pattern.
*/
template <typename ImplT>
class Image_Base
{
    public:

        /// @brief An STL-compatible iterator type.
        typedef Pixel_Iterator<      ImplT> iterator;

        /// @brief  An STL-compatible const-iterator type
        typedef Pixel_Iterator<const ImplT> const_iterator;


        /// Methods to access the derived type when sitting in the base
        ImplT&        impl()       { return static_cast<ImplT&>( *this ); }
        ImplT const&  impl() const { return static_cast<ImplT const&>( *this ); }

        /// Returns an iterator pointing to the first pixel in the image.
        iterator begin()             { return       iterator(impl(),0,0,0); }
        const_iterator begin() const { return const_iterator(impl(),0,0,0); }

        /// Returns an iterator pointing one past the last pixel in the image.
        iterator end()             { return       iterator(impl(),0,0,impl().planes()); }
        const_iterator end() const { return const_iterator(impl(),0,0,impl().planes()); }

        /**
         * Get the number of columns in the image.
        */
        size_t cols() const
        {
            return impl().cols();
        }

        /**
         * Get the number of rows in the image
        */
        size_t rows() const
        {
            return impl().rows();
        }

        /**
         * Get the number of channels in the image
        */
        size_t channels() const
        {
            return math::Compound_Channel_Count<typename ImplT::pixel_type>::value;
        };

        /**
         * Get the number of planes in the image
        */
        size_t planes() const
        {
            return impl().planes();
        }

        /**
         * Create a Bounding Box for the full image dimensions.  Nice
         * helper script for APIs that need math::Rect2i inputs.
        */
        virtual math::Rect2i full_bbox() const
        {
            return math::Rect2i( 0, 0, cols(), rows() );
        }

        /**
         * Get the Pixel-Format Enum from the Pixel-Type
        */
        Pixel_Format_Enum pixel_format() const
        {
            return Pixel_Format_ID<typename ImplT::pixel_type>::value;
        }

        /**
         * Get the channel-type from the image's pixel type
         *
         * Note, if you ever get UNKNOWN returned from this method, see Channel_Type_ID<> specification.
         * One likely scenario is you didn't register a template specialization of that class, for that channel-type.
         *
         * If the channel-type is basic, then use standard types or review the overrides after the custom channels in
         * that same file.
        */
        Channel_Type_Enum channel_type() const
        {
            typedef typename ImplT::pixel_type ptype; // Get the underlying datatype
            typedef typename math::Compound_Channel_Type<ptype>::type cp_type; // Resolve for if this is a compound or simple type
            return Channel_Type_ID<cp_type>::value;

            //return ChannelTypeID<typename CompoundChannelType<typename ImplT::pixel_type>::type>::value;
        }


        /// Returns an ImageFormat object describing the image format.
        Image_Format format() const
        {
            Image_Format format( impl().cols(),
                                 impl().rows(),
                                 impl().planes(),
                                 pixel_format(),
                                 channel_type(),
                                 true );

            return format;
        }

        /**
         * Get the list of interest points
         */
        feature::Interest_Point_List const& interest_points() const
        {
            return m_interest_points;
        }

        /**
         * Get the list of interest points
         */
        feature::Interest_Point_List& interest_points()
        {
            return m_interest_points;
        }

        /**
         * Get the internal metadata
         */
        meta::Metadata_Container_Base::ptr_t metadata() const
        {
            return m_metadata;
        }

        /**
         * Copy all non-pixel data.  As this capability grows, just put it all
         * here so the dozen or so View-Types don't have to overthink it.
         */
        template <typename ImageT>
        void copy_payload_data( const ImageT& rhs )
        {
            interest_points() = rhs.interest_points();
            metadata()        = rhs.metadata();
        }

        /**
         * Get this class name
        */
        static std::string class_name()
        {
            return "Image_Base";
        }

        static std::string full_name()
        {
            return "Image_Base<" + ImplT::class_name() + ">";
        }

    protected:

        /// The user can't be allowed to use these
        Image_Base() = default;

        Image_Base( const Image_Base& rhs ) = default;

        Image_Base& operator = ( const Image_Base& rhs )
        {
            return (*this);
        }

        /// List of feature points
        feature::Interest_Point_List m_interest_points;

        /// Image Metadata
        meta::Metadata_Container_Base::ptr_t m_metadata { std::make_shared<meta::Metadata_Container_Base>() };

}; // End of ImageBase Class

} // End of tmns::image Namespace
