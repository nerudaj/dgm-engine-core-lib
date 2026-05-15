#include "filesystem/TguiLoader.hpp"
#include <DGM/classes/Utility.hpp>

std::expected<tgui::Font, dgm::Error>
TguiLoader::loadFont(const std::filesystem::path& path)
{
    try
    {
        return tgui::Font(path.string());
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(dgm::Error(ex.what()));
    }
}

std::expected<tgui::Theme::Ptr, dgm::Error>
TguiLoader::loadTheme(const std::filesystem::path& path)
{
    try
    {
        return tgui::Theme::create(path.string());
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(dgm::Error(ex.what()));
    }
}

std::expected<tgui::Texture, dgm::Error>
TguiLoader::loadTexture(const std::filesystem::path& path)
{
    try
    {
        auto sfTexture = dgm::Utility::loadTexture(path);
        auto texture = tgui::Texture();
        texture.loadFromPixelData(
            sfTexture.value().getSize(),
            sfTexture.value().copyToImage().getPixelsPtr());
        return texture;
    }
    catch (const std::exception& ex)
    {
        return std::unexpected { dgm::Error(ex.what()) };
    }
}
