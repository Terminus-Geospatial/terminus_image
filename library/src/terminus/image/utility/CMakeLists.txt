#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/29/2023

#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/image/utility )

add_library( TERMINUS_IMAGE_UTIL OBJECT
                OpenCV_Utilities.cpp
                View_Utilities.cpp )