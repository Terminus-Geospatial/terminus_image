#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/13/2023

add_subdirectory( drivers )

#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/image/io )

add_library( TERMINUS_IMAGE_IO OBJECT
                Image_Resource_Disk.cpp )