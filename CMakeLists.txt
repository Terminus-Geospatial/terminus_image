############################# INTELLECTUAL PROPERTY RIGHTS #############################
##                                                                                    ##
##                           Copyright (c) 2024 Terminus LLC                          ##
##                                All Rights Reserved.                                ##
##                                                                                    ##
##          Use of this source code is governed by LICENSE in the repo root.          ##
##                                                                                    ##
############################# INTELLECTUAL PROPERTY RIGHTS #############################
#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    July 5, 2023
#
#    Purpose:  Build terminus-image API

#  Set CMake Version
cmake_minimum_required( VERSION 3.22 FATAL_ERROR )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )

# Set the Project-Name
project( ${CONAN_PKG_NAME}
            VERSION ${CONAN_PKG_VERSION}
            HOMEPAGE_URL ${CONAN_PKG_URL}
            DESCRIPTION ${CONAN_PKG_DESCRIPTION}
            LANGUAGES CXX )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#  Set C++ 23 Support
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Give Vscode a fighting chance
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#  Setup libdl for debug GCC builds
if( CMAKE_BUILD_TYPE STREQUAL "debug" OR CMAKE_BUILD_TYPE STREQUAL "Debug" )
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -ldl" )
    endif()
endif()

# Bring in CMake helpers for TERMINUS projects
include( terminus_cmake )

#-------------------------------------#
#-     Add Manual CMake Scripts      -#
#-------------------------------------#
LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

#  Look for dependencies
include( "${PROJECT_SOURCE_DIR}/cmake/scripts/Find_Dependencies.cmake" )

#  Add Interfaces
add_subdirectory( library )



terminus_lib_configure( ${PROJECT_NAME} )

#  Enable code coverage
if( TERMINUS_IMAGE_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

if( TERMINUS_IMAGE_ENABLE_TESTS )
    enable_testing()
    add_subdirectory( tests )
endif()

install( DIRECTORY library/include/terminus DESTINATION include)