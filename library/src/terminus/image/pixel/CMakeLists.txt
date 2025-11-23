#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/13/2023

include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/image/pixel )

add_library( TERMINUS_IMAGE_PIXEL OBJECT
             Channel_Type_Enum.cpp
             convert.cpp
             Pixel_Format_Enum.cpp )
