/**
 * @file    Detector_Config_OCV_ORB.hpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#pragma once

// Terminus Feature Libraries
#include "Detector_Config_OCV_Base.hpp"

namespace tmns::feature::ocv {

/**
 * Configuration options for the Oriented Brief Feature-Detector
*/
class Detector_Config_OCV_ORB : public Detector_Config_OCV_Base
{
    public:

        typedef std::shared_ptr<Detector_Config_OCV_ORB> ptr_t;

        /**
         * OpenCV Oriented Brief configuration instance.
         *
         * These parameters are lifted from the OpenCV documentation.
         * @see https://docs.opencv.org/3.4/db/d95/classcv_1_1ORB.html#ac499c951d4ed95733872ef7264096198
        */
        Detector_Config_OCV_ORB( const math::Size2i&  tile_size_pixels = math::Size2i( { 2048, 2048 } ),
                                 int                  max_features     = 500,
                                 float                scale_factor     = 1.2,
                                 int                  num_pyr_levels   = 8,
                                 int                  edge_threshold   = 31,
                                 int                  base_pyr_level   = 0,
                                 int                  wta_k            = 2,
                                 const std::string&   score_type       = "HARRIS",
                                 int                  patch_size       = 31,
                                 int                  fast_threshold   = 20 );

        /**
         * @brief Destructor
        */
        ~Detector_Config_OCV_ORB() = default;

        /**
         * @brief Support Feature Descriptors
         */
        bool supports_feature_descriptors() const override;

        /**
         * @brief Check if we allow custom tile sizes.(Yes this does)
         * 
         * This always returns TRUE
         */
        bool allow_custom_tile_size() const override; 

        /**
         * @brief Get the custom tile size in pixels
         */
        math::Size2i tile_size_pixels() const override;

        /**
         * @brief Get the max number of features.
         */
        int max_features() const override;

        /**
         * @brief Get the scale factor. 
         * 
         * From OpenCV Docs:   Pyramid decimation ratio, greater than 1. scaleFactor==2
         *                     means the classical pyramid, where each next level has 4x
         *                     less pixels than the previous, but such a big scale factor
         *                     will degrade feature matching scores dramatically. On the
         *                     other hand, too close to 1 scale factor will mean that to
         *                     cover certain scale range you will need more pyramid levels
         *                     and so the speed will suffer.
         */
        float scale_factor() const;

        /**
         * Number of pyramid levels
         * 
         * From OpenCV Docs: The number of pyramid levels. The smallest level will have 
         *                   linear size equal to input_image_linear_size/pow(scaleFactor, nlevels - firstLevel).
         */
        int num_pyr_levels() const;

        /**
         * @brief Get the edge threshold
         * 
         * From OpenCV Docs: This is size of the border where the features are not detected. It should roughly match
         *                   the patchSize parameter.
         */
        int edge_threshold() const;

        /**
         * @brief Get the Base Pyramid Level
         * 
         * From OpenCV Docs: The level of pyramid to put source image to. Previous layers are filled with upscaled 
         *                   source image.
        */
        int base_pyramid_level() const;

        /**
         * @brief Get the WTA-K value
         * 
         * From OpenCV Docs: The number of points that produce each element of the oriented 
         *                   BRIEF descriptor. The default value 2 means the BRIEF where we
         *                   take a random point pair and compare their brightnesses, so we
         *                   get 0/1 response. Other possible values are 3 and 4. For example,
         *                   3 means that we take 3 random points (of course, those point coordinates 
         *                   are random, but they are generated from the pre-defined seed, so each 
         *                   element of BRIEF descriptor is computed deterministically from the pixel
         *                   rectangle), find point of maximum brightness and output index of the winner 
         *                   (0, 1 or 2). Such output will occupy 2 bits, and therefore it will need 
         *                   a special variant of Hamming distance, denoted as NORM_HAMMING2 
         *                   (2 bits per bin). When WTA_K=4, we take 4 random points to compute 
         *                   each bin (that will also occupy 2 bits with possible values 0, 1, 2 or 3).
        */
        int wta_k() const;

        /**
         * @brief Get the Score Type
         * 
         * From OpenCV Docs:  The default HARRIS_SCORE means that Harris algorithm is used to rank
         *                    features (the score is written to KeyPoint::score and is used to retain
         *                    best nfeatures features); FAST_SCORE is alternative value of the parameter
         *                    that produces slightly less stable keypoints, but it is a little faster 
         *                    to compute.
        */
        std::string score_type() const;

        /**
         * @brief Get the Patch Size
         * 
         * From OpenCV Docs: Size of the patch used by the oriented BRIEF descriptor.
         *                   Of course, on smaller pyramid layers the perceived image area covered by a feature
         *                   will be larger.
         */
        int patch_size() const;

        /**
         * @brief Get the FAST Threshold
         * 
         * From OpenCV Docs: 
         */
        int fast_threshold() const;

        /**
         * Get the name to use for the detector's logger instance
        */
        std::string logger_name() const override;

        /**
         * Print to a logging string
        */
        std::string to_string( size_t offset = 0 ) const override;

    private:

        /// Tile Size in Pixels
        math::Size2i m_tile_size_pixels { math::Size2i( { 2048, 2048 } ) };

        /// Max Features to Retain
        int  m_max_features { 500 };

        /// Scale Factor
        double m_scale_factor { 1.2 };

        /// Number of Pyramid Levels
        int m_num_pyr_levels { 8 };

        /// Edge Threshold
        int m_edge_threshold { 31 };

        /// Base Pyramid Level
        int m_base_pyramid_level { 0 };

        /// WTA-K
        int m_wta_k { 2 };

        /// Score Type
        std::string m_score_type { "HARRIS" };

        /// Patch Size
        int m_patch_size { 31 };

        /// FAST Threshold
        int m_fast_threshold { 20 };

}; // End of Detector_Config_OCV_ORB class

} // End of tmns::feature::ocv namespace