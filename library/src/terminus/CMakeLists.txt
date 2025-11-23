#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/20/2023
#

#  Add include paths
include_directories( ${CMAKE_SOURCE_DIR}/include
                     ${Boost_INCLUDE_DIRS}
                     ${terminus_log_INCLUDE_DIRS}
                     ${terminus_math_INCLUDE_DIRS}
                     ${terminus_outcome_INCLUDE_DIRS}
                     ${GDAL_INCLUDE_DIRS}
)

add_subdirectory( feature )
add_subdirectory( geography )
add_subdirectory( image )
