/**
 * @file    TEST_file_io.cpp
 * @author  Marvin Smith
 * @date    7/22/2023
*/

// C++ Libraries
#include <filesystem>

// Terminus Libraries
#include <terminus/core/utility/terminal_progress_callback.hpp>
#include <terminus/log/configure.hpp>
#include <terminus/image/io/read_image_disk.hpp>
#include <terminus/image/io/write_image.hpp>
#include <terminus/image/utility/view_utilities.hpp>

namespace tx = tmns::image;

int main( int argc, char* argv[] )
{
    // Parse Command-Line Options
    std::filesystem::path input_image( argv[1] );
    std::filesystem::path output_image( argv[2] );

    // Load the image
    auto image_res = tx::io::read_image_disk<tx::PixelRGBA_f64>( input_image );

    // Create progress callback
    tmns::core::utility::Terminal_Progress_Callback callback( "terminus_image_tests",
                                                              "write_image_block result: " );


    // Try to rasterize the image completely
    tx::Image_Memory<tx::PixelRGBA_f64> mem_img = image_res.value();

    tx::utility::view_image( "Test", mem_img );

    // Write the image
    //auto write_status = timg::io::write_image( output_image,
    //                                           image_res.value(),
    //                                           std::map<std::string,std::string>(),
    //                                           timg::io::Disk_Driver_Manager::create_write_defaults(),
    //                                           callback );
}