set ( SFML_VERSION "3.1.0" )
set ( TGUI_VERSION "1.x" )
set ( DGM_LIB_VERSION "main" )
set ( RANGEV3_VERSION "master" )
set ( FMTLIB_VERSION "12.1.0" )

if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Android" )
	set ( BUILD_SHARED_LIBS ON )
endif()

set ( SFML_STATIC_LIBRARIES ${USE_SFML_TGUI_STATIC_LINKAGE} )
set ( SFML_USE_SYSTEM_DEPS OFF )
set ( SFML_USE_SYSTEM_DEPS_DEFAULT OFF )

CPMAddPackage("gh:SFML/SFML#${SFML_VERSION}")

set ( TGUI_BACKEND SFML_GRAPHICS )
set ( TGUI_STATIC_LIBRARIES ${USE_SFML_TGUI_STATIC_LINKAGE} )
CPMAddPackage("gh:texus/TGUI#${TGUI_VERSION}")

CPMAddPackage("gh:nerudaj/dgm-lib#${DGM_LIB_VERSION}")

if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Android" )
    CPMAddPackage("gh:fmtlib/fmt#${FMTLIB_VERSION}")
    
    CPMAddPackage(
        NAME range-v3
        GITHUB_REPOSITORY ericniebler/range-v3
        GIT_TAG ${RANGEV3_VERSION}
        GIT_SUBMODULES "test"
    )
endif ()
