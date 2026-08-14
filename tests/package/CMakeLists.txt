#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    8/14/2026
#
#    Purpose:  Build demo package
cmake_minimum_required( VERSION 4.0.0 FATAL_ERROR )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )

project( Image_Package_Test )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package( terminus_image REQUIRED )

add_executable( example example.cpp )

target_link_libraries( example terminus_image::terminus_image )
