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
 * @file    Detector_Base.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/log/Logger.hpp>

// Terminus Feature Libraries
#include "../image/types/Image_Base.hpp"
#include "../image/types/Image_Memory.hpp"
#include "Detector_Config_Base.hpp"
#include "Interest_Point.hpp"

// C++ Libraries
#include <memory>
#include <mutex>

namespace tmns::feature {

/**
 * Base class for all feature point detection modules.
*/
class Detector_Base
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Detector_Base> ptr_t;

        /**
         * Constructor given a configuration.  Null configuration will be bad,
         * so always provide a valid and usable configuration.
        */
        Detector_Base( Detector_Config_Base::ptr_t config );

        /**
         * Destructor
        */
        virtual ~Detector_Base() = default;

        /**
         * Function / Call Operator
        */
        template <typename PixelT>
        Result<Interest_Point_List> operator()( const image::Image_Memory<PixelT>&  image,
                                                bool                                cast_if_ptype_unsupported = true,
                                                int                                 max_points_override = 0)
        {
            {
                std::unique_lock<std::mutex> lck( m_log_mtx );
                std::stringstream sout;
                sout << "Computing interest points for region " << image.cols() << " x " << image.rows();
            }

            // Rasterize
            return this->process_image( image.buffer(),
                                        cast_if_ptype_unsupported,
                                        max_points_override );
        }

        /**
         * Function / Call Operator
         */
        template <typename ImageT>
        Result<Interest_Point_List> operator()( const image::Image_Base<ImageT>&  image,
                                                bool                              cast_if_ctype_unsupported = true,
                                                int                               max_points_override = 0 )
        {
            {
                std::unique_lock<std::mutex> lck( m_log_mtx );
                std::stringstream sout;
                sout << "Computing interest points for region " << image.cols() << " x " << image.rows();
                m_logger.trace( sout.str() );
            }

            // Rasterize to a memory image
            image::Image_Memory<typename ImageT::pixel_type> dest_image = image.impl(); 
            return this->operator()( dest_image,
                                     cast_if_ctype_unsupported,
                                     max_points_override );
        }

        /**
         * Process the image and detect keypoints
         */
        virtual Result<Interest_Point_List> process_image( const image::Image_Buffer& image_data,
                                                           bool                       cast_if_ctype_unsupported,
                                                           int                        max_points_override ) = 0;

        /**
         * Process the image to extract feature descriptors
         */
        virtual Result<void> perform_feature_extraction( const image::Image_Buffer&    image_data,
                                                         std::vector<Interest_Point>&  interest_point,
                                                         bool                          cast_if_ctype_unsupported );


        /**
         * Method to perfrom feature extraction
        */
        template <typename PixelT>
        Result<void> extract_descriptors( const image::Image_Memory<PixelT>&  image,
                                          std::vector<Interest_Point>&        interest_points,
                                          bool                                cast_if_ctype_unsupported = true )
        {
            {
                std::unique_lock<std::mutex> lck( m_log_mtx );
                std::stringstream sout;
                sout << "Extracting feature descriptors for region " << image.cols() << " x " << image.rows();
            }

            // Rasterize
            return this->perform_feature_extraction( image.buffer(),
                                                     interest_points,
                                                     cast_if_ctype_unsupported );
        }

        /**
         * Method to perfrom feature extraction
         */
        template <typename ImageT>
        Result<void> extract_descriptors( const image::Image_Base<ImageT>&  image,
                                          std::vector<Interest_Point>&      interest_points,
                                          bool                              cast_if_ctype_unsupported = true )
        {
            image::Image_Memory<typename ImageT::pixel_type> dest_image = image.impl(); 
            return this->extract_descriptors( dest_image,
                                              interest_points,
                                              cast_if_ctype_unsupported );
        }


        /**
         * Get the configuration
         */
        Detector_Config_Base::ptr_t config() const
        {
            return m_config;
        }

        /**
         * Get the class name
         */
        virtual std::string class_name() const = 0;

    protected: 

        /// Detector Logger Instance
        tmns::log::Logger m_logger;

        /// Logger Mutex
        std::mutex m_log_mtx;
        
    private:

        /// Internal Configuration Options
        Detector_Config_Base::ptr_t m_config;

}; // End of Detector_Base class

} // End of tmns::feature namespace