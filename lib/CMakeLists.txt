cmake_minimum_required ( VERSION 3.26 )

set ( TARGET dgm-engine-core-lib )

if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Android" )
    make_static_library (
        ${TARGET}
        DEPS
            dgm::dgm-lib
            TGUI::TGUI
            SFML::Audio
            fmt::fmt
            range-v3::range-v3
    )
else ()
    make_static_library (
        ${TARGET}
        DEPS
            dgm::dgm-lib
            TGUI::TGUI
            SFML::Audio
    )
endif ()

target_compile_features ( ${TARGET} PRIVATE cxx_std_23 )
set_target_properties ( ${TARGET} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if ( UNIX )
    target_compile_definitions ( ${TARGET} PRIVATE LINUX=1 )
endif ()

target_precompile_headers( ${TARGET}
    PUBLIC
        <vector>
        <string>
        <algorithm>
        <functional>
        <utility>
        <variant>
        <tuple>
        <iostream>
        <fstream>
        <map>
        <unordered_map>
        <set>
)