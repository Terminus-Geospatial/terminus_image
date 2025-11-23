#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    8/26/2023


#  Build this library
include_directories( ${CMAKE_SOURCE_DIR}/include/terminus/image/collection/file )

add_library( TERMINUS_IMAGE_COLLECT_FILE OBJECT
                File_Collection_Utilities.cpp
                TOML_Utilities.cpp )