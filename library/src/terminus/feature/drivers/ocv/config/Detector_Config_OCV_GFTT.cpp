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
 * @file    Detector_Config_OCV_GFTT.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include "Detector_Config_OCV_GFTT.hpp"

// C++ Libraries
#include <sstream>

namespace tmns::feature::ocv {

/*********************************/
/*          Constructor          */
/*********************************/
Detector_Config_OCV_GFTT::Detector_Config_OCV_GFTT( const math::Size2i&  tile_size_pixels,
                                                    int                  max_features,
                                                    double               quality_level,
                                                    double               min_distance,
                                                    int                  block_size,
                                                    bool                 use_harris_detector,
                                                    double               k )
  : Detector_Config_OCV_Base(),
    m_tile_size_pixels( tile_size_pixels ),
    m_max_features( max_features ),
    m_quality_level( quality_level ),
    m_min_distance( min_distance ),
    m_block_size( block_size ),
    m_use_harris_detector( use_harris_detector ),
    m_k( k )
{
}

/************************************/
/*      Allow Custom Tile Size      */
/************************************/
bool Detector_Config_OCV_GFTT::allow_custom_tile_size() const
{
    return true;
}

/********************************/
/*      Tile Size in Pixels     */
/********************************/
math::Size2i  Detector_Config_OCV_GFTT::tile_size_pixels() const
{
    return m_tile_size_pixels;
}

/****************************/
/*      Get Max Corners     */
/****************************/
int Detector_Config_OCV_GFTT::max_features() const
{
    return m_max_features;
}

/*******************************/
/*      Get Quality Level      */
/*******************************/
double Detector_Config_OCV_GFTT::quality_level() const
{
    return m_quality_level;
}

/**************************/
/*      Min Distance      */
/**************************/
double Detector_Config_OCV_GFTT::min_distance() const
{
    return m_min_distance;
}

/************************/
/*      Block Size      */
/************************/
int Detector_Config_OCV_GFTT::block_size() const
{
    return m_block_size;
}

/*****************************************/
/*          Use Harris Detector          */
/*****************************************/
bool Detector_Config_OCV_GFTT::use_harris_detector() const
{
    return m_use_harris_detector;
}

/******************/
/*      Get K     */
/******************/
double Detector_Config_OCV_GFTT::k() const
{
    return m_k;
}

/**************************************/
/*      Print to logging string       */
/**************************************/
std::string Detector_Config_OCV_GFTT::logger_name() const
{
    return "Detector_OCV_GFTT";
}

/********************************************/
/*          Print to Logger String          */
/********************************************/
std::string Detector_Config_OCV_GFTT::to_string( size_t offset ) const
{
    std::string gap( offset, ' ' );
    std::stringstream sout;
    sout << gap << " - " << logger_name() << std::endl;
    sout << gap << "    - allow custom tile size: " << std::boolalpha << allow_custom_tile_size() << std::endl;
    sout << gap << "    - tile size : " << tile_size_pixels().to_string() << std::endl;
    sout << gap << "    - max_features: " << max_features() << std::endl;
    sout << gap << "    - quality_level: " << quality_level() << std::endl;
    sout << gap << "    - min_distance: " << min_distance() << std::endl;
    sout << gap << "    - block_size: " << block_size() << std::endl;
    sout << gap << "    - use_harris_detector: " << std::boolalpha << use_harris_detector() << std::endl;
    sout << gap << "    - k: " << std::fixed << k() << std::endl;

    return sout.str();
}

} // End of tmns::feature::ocv namespace