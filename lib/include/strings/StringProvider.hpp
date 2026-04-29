#pragma once

#include "strings/Language.hpp"
#include "types/StringTypes.hpp"
#include <string>
#include <utility>
#include <vector>
#include <type_traits>
#include <map>

template<class E>
concept ScopedEnum = std::is_scoped_enum_v<E>;

template<ScopedEnum Language, ScopedEnum StringId>
class [[nodiscard]] StringProvider final
{
public:
    using Localization = std::map<StringId, std::string_view>;

public:
    explicit StringProvider(
        Language primary,
        const std::map<Language, Localization>& localizations)
    : current(primary)
    , strings(localizations)
    {
        // For untranslated strings, use strings from the primary language
        for (auto&& [lang, localization] : strings)
        {
            for (auto&& idx : std::views::iota(size_t {}, localization.size()))
            {
                if (localization[idx].empty())
                    localization[idx] = strings[std::to_underlying(primary)][idx];
            }
        }
    }

    StringProvider(StringProvider&&) = delete;
    StringProvider(const StringProvider&) = delete;

public:
    constexpr void changeLanguage(Language newLang) noexcept
    {
        current = newLang;
    }

    [[nodiscard]] constexpr const CharType*
    getString(StringId id) const noexcept
    {
        const auto result =
            strings[std::to_underlying(current)][std::to_underlying(id)].data();
        if (!result) return "--missing string--";
        return result;
    }

private:
    Language current = Language {};
    std::map<Language, Localization> strings;
};
