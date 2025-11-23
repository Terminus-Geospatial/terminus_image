#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/13/2023
add_subdirectory( gdal )
#add_subdirectory( nitf )

include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/image/io/drivers )

add_library( TERMINUS_IMAGE_IO_DRIVERS OBJECT
             Disk_Driver_Manager.cpp
)
