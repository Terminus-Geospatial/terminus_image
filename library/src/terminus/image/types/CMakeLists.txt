#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/22/2023

#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/image/types )

add_library( TERMINUS_IMAGE_TYPES OBJECT
                Image_Buffer.cpp
                Image_Format.cpp
                Image_Resource_Base.cpp )