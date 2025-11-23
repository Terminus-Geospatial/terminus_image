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
add_subdirectory( include/terminus )
add_subdirectory( src/terminus )

#  Create Library
add_library( ${PROJECT_NAME} SHARED
    ${TERMINUS_IMAGE_INCLUDE_HEADERS}
    $<TARGET_OBJECTS:TERMINUS_GEOGRAPHY_CAMERA>
    $<TARGET_OBJECTS:TERMINUS_FEATURE_BASE>
    $<TARGET_OBJECTS:TERMINUS_FEATURE_DRV_OCV>
    $<TARGET_OBJECTS:TERMINUS_FEATURE_DRV_OCV_CFG>
    $<TARGET_OBJECTS:TERMINUS_FEATURE_UTIL>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_COLLECT_FILE>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_IO>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_IO_DRIVERS>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_IO_DRIVERS_GDAL>
#    $<TARGET_OBJECTS:TERMINUS_IMAGE_IO_DRIVERS_NITF>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_METADATA>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_PIXEL>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_TYPES>
    $<TARGET_OBJECTS:TERMINUS_IMAGE_UTIL>
)

#  Link in dependencies
target_link_libraries( ${PROJECT_NAME} PUBLIC
     Boost::headers
     Boost::log
     Boost::log_setup
     Boost::stacktrace
     Boost::system
     GDAL::GDAL
     nlohmann_json::nlohmann_json
     ${OpenCV_LIBS}
     terminus_core::terminus_core
     terminus_log::terminus_log
     terminus_math::terminus_math
     terminus_outcome::terminus_outcome
)

install( TARGETS ${PROJECT_NAME} )

terminus_lib_configure( ${PROJECT_NAME} )

#  Enable code coverage
if( TERMINUS_IMAGE_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

if( TERMINUS_IMAGE_ENABLE_TESTS )
    enable_testing()
    add_subdirectory( tests )
endif()

install( DIRECTORY include/terminus DESTINATION include)