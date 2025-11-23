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
 * @file    Detector_Config_OCV_ORB.cpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#include "Detector_Config_OCV_ORB.hpp"

// C++ Libraries
#include <sstream>

namespace tmns::feature::ocv {

/*********************************/
/*          Constructor          */
/*********************************/
Detector_Config_OCV_ORB::Detector_Config_OCV_ORB( const math::Size2i&  tile_size_pixels,
                                                  int                  max_features,
                                                  float                scale_factor,
                                                  int                  num_pyr_levels,
                                                  int                  edge_threshold,
                                                  int                  base_pyr_level,
                                                  int                  wta_k,
                                                  const std::string&   score_type,
                                                  int                  patch_size,
                                                  int                  fast_threshold )
  : Detector_Config_OCV_Base(),
    m_tile_size_pixels( tile_size_pixels ),
    m_max_features( max_features ),
    m_scale_factor( scale_factor ),
    m_num_pyr_levels( num_pyr_levels ),
    m_edge_threshold( edge_threshold ),
    m_base_pyramid_level( base_pyr_level ),
    m_wta_k( wta_k ),
    m_score_type( score_type ),
    m_patch_size( patch_size ),
    m_fast_threshold( fast_threshold )
{
}

/**********************************************************/
/*          Check if Descriptors are supported            */
/**********************************************************/
bool Detector_Config_OCV_ORB::supports_feature_descriptors() const
{
    return true;
}

/************************************/
/*      Allow Custom Tile Size      */
/************************************/
bool Detector_Config_OCV_ORB::allow_custom_tile_size() const
{
    return true;
}

/********************************/
/*      Tile Size in Pixels     */
/********************************/
math::Size2i  Detector_Config_OCV_ORB::tile_size_pixels() const
{
    return m_tile_size_pixels;
}

/*****************************/
/*      Get Max Features     */
/*****************************/
int Detector_Config_OCV_ORB::max_features() const
{
    return m_max_features;
}

/*******************************/
/*      Get Scale Factor       */
/*******************************/
float Detector_Config_OCV_ORB::scale_factor() const
{
    return m_scale_factor;
}

/***********************************/
/*      Number Pyramid Levels      */
/***********************************/
int Detector_Config_OCV_ORB::num_pyr_levels() const
{
    return m_num_pyr_levels;
}

/****************************/
/*      Edge Threshold      */
/****************************/
int Detector_Config_OCV_ORB::edge_threshold() const
{
    return m_edge_threshold;
}

/*****************************************/
/*          Base Pyramid Level           */
/*****************************************/
int Detector_Config_OCV_ORB::base_pyramid_level() const
{
    return m_base_pyramid_level;
}

/******************/
/*      WTA-K     */
/******************/
int Detector_Config_OCV_ORB::wta_k() const
{
    return m_wta_k;
}

/*********************************/
/*          Score Type           */
/*********************************/
std::string Detector_Config_OCV_ORB::score_type() const
{
    return m_score_type;
}

/*********************************/
/*          Patch Size           */
/*********************************/
int Detector_Config_OCV_ORB::patch_size() const
{
    return m_patch_size;
}

/*************************************/
/*          Fast Threshold           */
/*************************************/
int Detector_Config_OCV_ORB::fast_threshold() const
{
    return m_fast_threshold;
}

/**************************************/
/*      Print to logging string       */
/**************************************/
std::string Detector_Config_OCV_ORB::logger_name() const
{
    return "Detector_OCV_ORB";
}

/********************************************/
/*          Print to Logger String          */
/********************************************/
std::string Detector_Config_OCV_ORB::to_string( size_t offset ) const
{
    std::string gap( offset, ' ' );
    std::stringstream sout;
    sout << gap << " - " << logger_name() << std::endl;
    sout << gap << "    - allow custom tile size: " << std::boolalpha << allow_custom_tile_size() << std::endl;
    sout << gap << "    - tile size : " << tile_size_pixels().to_string() << std::endl;
    sout << gap << "    - max_features: " << max_features() << std::endl;
    sout << gap << "    - scale factor: " << std::fixed << scale_factor() << std::endl;
    sout << gap << "    - num pyr levels: " << num_pyr_levels() << std::endl;
    sout << gap << "    - edge threshold: " << edge_threshold() << std::endl;
    sout << gap << "    - base pyramid level: " << base_pyramid_level() << std::endl;
    sout << gap << "    - wta_k: " << wta_k() << std::endl;
    sout << gap << "    - score_type: " << score_type() << std::endl;
    sout << gap << "    - patch size: " << patch_size() << std::endl;
    sout << gap << "    - fast threshold: " << fast_threshold() << std::endl;

    return sout.str();
}

} // End of tmns::feature::ocv namespace