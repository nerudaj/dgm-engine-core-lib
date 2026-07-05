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
                ? "parent.width / 2 - width / 2"
                : "parent.width - width",
        valign == tgui::VerticalAlignment::Top
            ? "0%"
            : valign == tgui::VerticalAlignment::Center
                ? "parent.height / 2 - height / 2"
                : "parent.height - height",
    });
    // clang-format on
}
