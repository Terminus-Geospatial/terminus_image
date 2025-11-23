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
 * @file    Image_Resource_Disk.hpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>

// Terminus Image Libraries
#include "../types/Image_Resource_Base.hpp"

// Boost Libraries
#include <boost/utility.hpp>

// C++ Libraries
#include <filesystem>
#include <memory>

namespace tmns::image::io {

/**
 * Disk/Driver loading interface
*/
class Image_Resource_Disk : public Image_Resource_Base,
                                   boost::noncopyable
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Image_Resource_Disk> ptr_t;

        ~Image_Resource_Disk() override = default;

        /**
         * Return the name of the resource
        */
        virtual std::string resource_name() const = 0;

        /**
         * Return the filename of the disk image file.
         */
        std::filesystem::path pathname() const;

        /**
         * Specify if we should rescale when converting pixel types.
        */
        void set_rescale( bool rescale );

        /**
         * Specify global default for rescaling.
         */
        static void set_default_rescale( bool rescale );

        /**
         * Print to log-friendly string
        */
        virtual std::string to_log_string( size_t offset ) const = 0;

    protected:

        /**
         * Constructor
         * @param pathname Image to load.
        */
        Image_Resource_Disk( const std::filesystem::path& pathname );

        /// Pathname
        std::filesystem::path m_pathname;

        /// Flag to control if we rescale values during channel/pixel conversions.
        bool        m_rescale;

        /// Flag for default behavior [Initialized to true]
        static bool s_default_rescale;

}; // End of Image_Resource_Disk class


} // end of tmns::image::io namespace