cmake_minimum_required ( VERSION 3.31 )

project ( dgm-engine-core-lib )

option ( DGM_CORE_BOOTSTRAP_CPM "Whether to automatically download CPM" OFF )

include ( cmake/cpp-init.cmake )

if ( DGM_CORE_BOOTSTRAP_CPM AND NOT COMMAND CPMAddPackage )
    bootstrap_cpm()
endif()

include ( cmake/dependencies.cmake )

add_subdirectory ( lib )
