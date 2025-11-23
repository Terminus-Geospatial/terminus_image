#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/14/2023

include_directories( ${CMAKE_SOURCE_DIR}/include )

add_library( TERMINUS_IMAGE_IO_DRIVERS_NITF OBJECT
             Image_Resource_Disk_NITF.cpp
             Image_Resource_Disk_NITF_Factory.cpp
)
