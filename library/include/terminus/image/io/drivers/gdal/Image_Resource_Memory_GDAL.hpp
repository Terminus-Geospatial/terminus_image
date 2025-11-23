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
 * @file     Image_Resource_Memory_GDAL.hpp
 * @author   Marvin Smith
 * @date     7/13/2023
*/
#pragma once


namespace tmns::image::io::gdal {

/**
 * GDAL Driver for loading data in memory
*/
class Read_Image_Resource_Memory_GDAL : public Read_Image_Resource_Memory_Base,
                                               boost::noncopyable
{
    public:

        /**
         * Read image from memory
        */
        Result<Image_Buffer> read( const math::Rect2i bbox ) const;

}; // End of Read_Image_Resource_Memory_GDAL class

} // End of tmns::image::io::gdal