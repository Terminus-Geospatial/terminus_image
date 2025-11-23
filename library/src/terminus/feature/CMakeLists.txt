#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/30/2023

add_subdirectory( drivers )
add_subdirectory( utility )

#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/feature )

add_library( TERMINUS_FEATURE_BASE OBJECT
                Detector_Base.cpp
                Detector_Config_Base.cpp
                Detector_Factory.cpp
                Interest_Point.cpp )