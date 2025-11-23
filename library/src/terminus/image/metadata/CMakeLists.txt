#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    9/16/2023


#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/image/metadata )

add_library( TERMINUS_IMAGE_METADATA OBJECT
                Metadata_Container_Base.cpp )