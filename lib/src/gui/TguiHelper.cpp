#include "gui/TguiHelper.hpp"

tgui::Texture TguiHelper::convertTexture(const sf::Texture& texture)
{
    auto result = tgui::Texture();
    result.loadFromPixelData(
        texture.getSize(), texture.copyToImage().getPixelsPtr());
    return result;
}

tgui::Texture TguiHelper::convertTexture(
    const sf::Texture& texture, const sf::IntRect& clipping)
{
    auto result = tgui::Texture();
    result.loadFromPixelData(
        texture.getSize(),
        texture.copyToImage().getPixelsPtr(),
        tgui::UIntRect(
            tgui::Vector2u(clipping.position.x, clipping.position.y),
            tgui::Vector2u(clipping.size.x, clipping.size.y)));
    return result;
}

void TguiHelper::alignInParent(
    tgui::Container::Ptr container,
    tgui::HorizontalAlignment align,
    tgui::VerticalAlignment valign)
{
    // clang-format off
    container->setPosition({
        align == tgui::HorizontalAlignment::Left
            ? "0%"
            : align == tgui::HorizontalAlignment::Center
                ? "50% - width / 2"
                : "100% - width",
        valign == tgui::VerticalAlignment::Top
            ? "0%"
            : valign == tgui::VerticalAlignment::Center
                ? "50% - height / 2"
                : "100% - height",
    });
    // clang-format on
}
