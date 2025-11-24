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
#    Date:    November 23, 2024
#
#    Purpose:  Consolidated build for terminus-image library

# Set global include directories for all terminus-image headers
include_directories( ${CMAKE_SOURCE_DIR}/library/include )

#  Find all header files in `./include` to ship with library
file( GLOB_RECURSE TERMINUS_IMAGE_INCLUDE_HEADERS
      CONFIGURE_DEPENDS
      "include/*.hpp" )

#  Create Library
add_library( ${PROJECT_NAME} SHARED
    ${TERMINUS_IMAGE_INCLUDE_HEADERS}
    src/terminus/geography/camera/camera_model_adjusted.cpp
    src/terminus/geography/camera/camera_model_base.cpp
    src/terminus/geography/camera/camera_model_isis.cpp
    src/terminus/geography/camera/camera_model_factory.cpp
    src/terminus/geography/camera/camera_model_pinhole.cpp
    src/terminus/geography/camera/distortion_base.cpp
    src/terminus/geography/camera/distortion_brown_conrady.cpp
    src/terminus/geography/camera/distortion_null.cpp
    src/terminus/feature/detector_base.cpp
    src/terminus/feature/detector_config_base.cpp
    src/terminus/feature/detector_factory.cpp
    src/terminus/feature/interest_point.cpp
    src/terminus/feature/utility/interest_point_write_task.cpp
    src/terminus/feature/drivers/ocv/detector_ocv_base.cpp
    src/terminus/feature/drivers/ocv/detector_ocv_gftt.cpp
    src/terminus/feature/drivers/ocv/detector_ocv_orb.cpp
    src/terminus/feature/drivers/ocv/opencv_detector_utilities.cpp
    src/terminus/feature/drivers/ocv/config/detector_config_ocv_base.cpp
    src/terminus/feature/drivers/ocv/config/detector_config_ocv_gftt.cpp
    src/terminus/feature/drivers/ocv/config/detector_config_ocv_orb.cpp
    src/terminus/image/collection/file/file_collection_utilities.cpp
    src/terminus/image/collection/file/toml_utilities.cpp
    src/terminus/image/io/image_resource_disk.cpp
    src/terminus/image/io/drivers/disk_driver_manager.cpp
    src/terminus/image/io/drivers/memory_driver_manager.cpp
    src/terminus/image/io/drivers/gdal/gdal_codes.cpp
    src/terminus/image/io/drivers/gdal/gdal_disk_image_impl.cpp
    src/terminus/image/io/drivers/gdal/gdal_utilities.cpp
    src/terminus/image/io/drivers/gdal/isis_json_parser.cpp
    src/terminus/image/io/drivers/gdal/image_resource_disk_gdal.cpp
    src/terminus/image/io/drivers/gdal/image_resource_disk_gdal_factory.cpp
    #src/terminus/image/io/drivers/nitf/image_resource_disk_nitf.cpp
    #src/terminus/image/io/drivers/nitf/image_resource_disk_nitf_factory.cpp
    src/terminus/image/metadata/metadata_container_base.cpp
)

#  Link in dependencies
target_link_libraries( ${PROJECT_NAME} PUBLIC
     Boost::headers
     Boost::log
     Boost::log_setup
     Boost::stacktrace
     GDAL::GDAL
     nlohmann_json::nlohmann_json
     ${OpenCV_LIBS}
     terminus_core::terminus_core
     terminus_log::terminus_log
     terminus_math::terminus_math
     terminus_outcome::terminus_outcome
)

install( TARGETS ${PROJECT_NAME} )