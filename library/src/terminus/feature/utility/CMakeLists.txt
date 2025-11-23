#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    8/31/2023

#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/feature/utility )

add_library( TERMINUS_FEATURE_UTIL OBJECT
             Interest_Point_Write_Task.cpp )
