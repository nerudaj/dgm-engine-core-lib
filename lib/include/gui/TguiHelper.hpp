#pragma once

#include "misc/Compatibility.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

class TguiHelper final
{
public:
    [[nodiscard]] static tgui::Texture
    convertTexture(const sf::Texture& texture);

    [[nodiscard]] static tgui::Texture
    convertTexture(const sf::Texture& texture, const sf::IntRect& clipping);

    static void alignInParent(
        tgui::Container::Ptr container,
        tgui::HorizontalAlignment align,
        tgui::VerticalAlignment valign);

    static inline void centerInParent(tgui::Container::Ptr container)
    {
        alignInParent(
            container,
            tgui::HorizontalAlignment::Center,
            tgui::VerticalAlignment::Center);
    }
};
