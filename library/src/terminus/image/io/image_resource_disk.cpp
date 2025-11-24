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
 * @file    image_resource_disk.cpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#include <terminus/image/io/image_resource_disk.hpp>

namespace tmns::image::io {

// Initialize Static Variables
bool Image_Resource_Disk::s_default_rescale = true;

/************************************/
/*          Get the pathname        */
/************************************/
std::filesystem::path Image_Resource_Disk::pathname() const
{
    return m_pathname;
}

/************************************/
/*          Constructor             */
/************************************/
Image_Resource_Disk::Image_Resource_Disk( const std::filesystem::path& pathname )
  : m_pathname( pathname ),
    m_rescale( s_default_rescale )
{
}

/********************************/
/*    Set the rescale factor    */
/********************************/
void Image_Resource_Disk::set_rescale( bool rescale )
{
    m_rescale = rescale;
}

/****************************************/
/*    Set the default rescale factor    */
/****************************************/
void Image_Resource_Disk::set_default_rescale(bool rescale)
{
    s_default_rescale = rescale;
}

} // end of tmns::image::io namespace