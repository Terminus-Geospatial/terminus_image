#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/30/2023

#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/feature/drivers/ocv/config )

add_library( TERMINUS_FEATURE_DRV_OCV_CFG OBJECT
                Detector_Config_OCV_Base.cpp
                Detector_Config_OCV_GFTT.cpp
                Detector_Config_OCV_ORB.cpp )
