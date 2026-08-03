#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/10/2023

#  Configure GoogleTest
find_package( GTest REQUIRED )
include( GoogleTest )

include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/src )

set( TEST ${PROJECT_NAME}_test )
add_executable( ${TEST}
    main.cpp
    TEST_scratch.cpp
    TEST_tomlplusplus.cpp
    feature/TEST_detect_interest_points.cpp
    feature/TEST_extract_descriptors.cpp
    feature/drivers/ocv/TEST_Detector_OCV_GFTT.cpp
    feature/drivers/ocv/TEST_Detector_OCV_ORB.cpp
    feature/drivers/ocv/TEST_ocv_gftt.cpp
    feature/drivers/ocv/TEST_ocv_orb.cpp
    geography/camera/TEST_Camera_Model_Factory.cpp
    image/collection/TEST_Collection_Resource_File.cpp
    image/io/TEST_read_image_disk.cpp
#    image/io/TEST_read_image.cpp
    image/io/TEST_read_write_battery.cpp
    image/io/drivers/gdal/TEST_GDAL_Codes.cpp
    image/io/drivers/gdal/TEST_GDAL_Utilities.cpp
    image/io/drivers/gdal/TEST_Image_Resource_Disk_GDAL.cpp
    image/io/drivers/gdal/TEST_Image_Resource_Disk_GDAL_Factory.cpp
    image/operations/drawing/TEST_compute_line_points.cpp
    image/operations/drawing/TEST_compute_circle_points.cpp
    image/operations/drawing/TEST_drawing_functions.cpp
    image/operations/TEST_crop_image.cpp
    image/operations/TEST_select_plane.cpp
    image/pixel/TEST_convert.cpp
    image/pixel/TEST_Pixel_Cast_Utilities.cpp
    image/types/TEST_Compound_Types.cpp
    image/types/TEST_Image_Disk.cpp
    image/types/TEST_Image_Resource_View.cpp
    image/types/TEST_Fundamental_Types.cpp
    image/types/TEST_Image_Memory.cpp
    UNIT_TEST_ONLY/Image_Datastore.cpp
    UNIT_TEST_ONLY/Image_Datastore.hpp
    UNIT_TEST_ONLY/Options.cpp
    UNIT_TEST_ONLY/Options.hpp
    UNIT_TEST_ONLY/Prerasterization_Test_View.hpp
    UNIT_TEST_ONLY/Test_Environment.cpp
    UNIT_TEST_ONLY/Test_Environment.hpp
)

target_link_libraries( ${TEST} PRIVATE
    GTest::gtest
    ${PROJECT_NAME}
)

gtest_discover_tests( ${TEST} )

#  Copy Unit-Test Data
file( COPY ${CMAKE_SOURCE_DIR}/data/unit-test-data/collection
      DESTINATION ${CMAKE_BINARY_DIR}/data )

file( COPY ${CMAKE_SOURCE_DIR}/data/unit-test-data/images
      DESTINATION ${CMAKE_BINARY_DIR}/data )

file( COPY ${CMAKE_SOURCE_DIR}/data/unit-test-data/test_images.ini
      DESTINATION ${CMAKE_BINARY_DIR}/data )

