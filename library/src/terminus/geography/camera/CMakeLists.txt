#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    9/15/2023
#  
#    Purpose:  Build the Geography Camera APIs


#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/geography/camera )

add_library( TERMINUS_GEOGRAPHY_CAMERA OBJECT
                Camera_Model_Adjusted.cpp
                Camera_Model_Base.cpp
                Camera_Model_ISIS.cpp
                Camera_Model_Factory.cpp
                Camera_Model_Pinhole.cpp
                Distortion_Base.cpp
                Distortion_Brown_Conrady.cpp
                Distortion_Null.cpp )