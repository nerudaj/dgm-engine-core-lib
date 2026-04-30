#pragma once

#include "types/StringTypes.hpp"
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using UnderlyingLanguageType = int;

template<class E>
concept ScopedEnum = std::is_scoped_enum_v<E>;

template<ScopedEnum StringId>
using Localization = std::map<StringId, std::string_view>;

template<class E>
concept LanguageEnum =
    std::is_scoped_enum_v<E>
    && std::is_same_v<UnderlyingLanguageType, std::underlying_type_t<E>>;

template<ScopedEnum StringId>
class [[nodiscard]] StringProvider final
{
public:

public:
    template<LanguageEnum Language>
    explicit StringProvider(
        Language primary,
        const std::map<Language, Localization<StringId>>& localizations)
        : current(std::to_underlying(primary))
    {
        for (auto&& [language, localization] : localizations)
        {
            for (auto&& [id, str] : localization)
            {
                strings[std::to_underlying(language)][id] = str;
            }
        }
    }

    StringProvider(StringProvider&&) = delete;
    StringProvider(const StringProvider&) = delete;

public:
    template<LanguageEnum Language>
    constexpr void changeLanguage(Language newLang) noexcept
    {
        current = newLang;
    }

    [[nodiscard]] constexpr const CharType*
    getString(StringId id) const noexcept
    {
        if (!strings.contains(current))
            return "--missing localization--";
        else if (!strings.at(current).contains(id))
            return "--missing string--";
        return strings.at(current).at(id).data();
    }

private:
    UnderlyingLanguageType current = {};
    std::map<UnderlyingLanguageType, Localization<StringId>> strings;
};
