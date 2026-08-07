#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/22/2023

include_directories( ${CMAKE_SOURCE_DIR}/library/include )
include_directories( ${CMAKE_SOURCE_DIR}/library/src )

function( add_component_test SUFFIX FILE )
    set( TEST test_comp_${PROJECT_NAME}_${SUFFIX} )
    add_executable( ${TEST} ${FILE} )
    target_link_libraries( ${TEST} PRIVATE ${PROJECT_NAME} )
endfunction()


add_component_test( file_io TEST_file_io.cpp )