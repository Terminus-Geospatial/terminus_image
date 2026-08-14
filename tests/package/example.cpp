/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2026 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    example.cpp
 * @author  Marvin Smith
 * @date    8/14/2026
 */

// Terminus Libraries
#include <terminus/image/exports.hpp>
#include <terminus/image/types/image_format.hpp>

// C++ Standard Libraries
#include <iostream>
#include <sstream>

int main()
{
    // 1 - Build a basic RGB/UINT8 image format and exercise a few accessors.
    tmns::image::Image_Format format( /* cols    = */ 640,
                                       /* rows    = */ 480,
                                       /* planes  = */ 1,
                                       tmns::image::Pixel_Format_Enum::RGB,
                                       tmns::image::Channel_Type_Enum::UINT8,
                                       /* premultiply = */ false );

    std::cout << "Image format: " << format.to_string() << std::endl;
    std::cout << "  cols:              " << format.cols() << std::endl;
    std::cout << "  rows:              " << format.rows() << std::endl;
    std::cout << "  channels:          " << format.channels() << std::endl;
    std::cout << "  raster size bytes: " << format.raster_size_bytes() << std::endl;

    if( format.cols() != 640 || format.rows() != 480 )
    {
        std::cerr << "Image_Format did not report the expected dimensions." << std::endl;
        return 1;
    }

    // 2 - Get build information and log this
    auto build_info = tmns::image::get_build_info();
    std::stringstream ss;
    ss << "Terminus Image Build Information:\n";
    for( const auto& [key, value] : build_info )
    {
        ss << key << ": " << value << "\n";
    }
    std::cout << ss.str() << std::endl;

    return 0;
}
