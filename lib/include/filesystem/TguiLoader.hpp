#pragma once

#include <DGM/classes/Error.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <expected>
#include <filesystem>

class [[nodiscard]] TguiLoader final {
public:
    static std::expected<tgui::Font, dgm::Error>
    loadFont(const std::filesystem::path& path);

    static std::expected<tgui::Theme::Ptr, dgm::Error>
    loadTheme(const std::filesystem::path& path);

    static std::expected<tgui::Texture, dgm::Error>
    loadTexture(const std::filesystem::path& path);
};
