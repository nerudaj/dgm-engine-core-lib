#pragma once

#include <DGM/classes/Compatibility.hpp>

#ifdef ANDROID
#include <fmt/core.h>
#include <range/v3/all.hpp>
#else
#include <format>
#include <ranges>
#endif

namespace uni
{
    #ifdef ANDROID
    using namespace fmt;

    namespace views {
        using ::ranges::views;
    }

    namespace ranges 
    {
        using namespace ::ranges;
    }
    #else
    using namespace std;
    #endif
}
