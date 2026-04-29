cmake_minimum_required ( VERSION 3.26 )

include ( cmake/cpp-init.cmake )

project ( dgm-engine-core-lib )

bootstrap_cpm()
include ( cmake/dependencies.cmake )

add_subdirectory ( lib )
