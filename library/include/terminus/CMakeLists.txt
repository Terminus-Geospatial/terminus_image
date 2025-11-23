#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/10/2023

set( TERMINUS_IMAGE_INCLUDE_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/image/pixel/Pixel_Base.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/image/pixel/Pixel_RGB.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/image/types/Image_Base.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/image/types/Image_Memory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/image/io/read_image.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/image/utility/View_Utilities.hpp
        PARENT_SCOPE )