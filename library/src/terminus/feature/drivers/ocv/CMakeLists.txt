#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/30/2023

add_subdirectory( config )

#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/feature/drivers/ocv )

add_library( TERMINUS_FEATURE_DRV_OCV OBJECT
                Detector_OCV_Base.cpp
                Detector_OCV_GFTT.cpp
                Detector_OCV_ORB.cpp )
