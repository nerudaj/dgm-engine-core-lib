#include "gui/builders/DefaultLayoutBuilder.hpp"
#include "gui/TguiHelper.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace priv
{
    priv::BuilderProperties
    DefaultLayoutBuilderHelper::buildProperties(const Sizer& sizer)
    {
        const auto baseHeight = sizer.getBaseContainerHeight();

        return priv::BuilderProperties {
            .baseHeight = baseHeight,
            .cornerButtonDimension = baseHeight * 3,
            .cornerButtonPadding = baseHeight / 2,
            .titleHeight = 3 * baseHeight,
        };
    }

    tgui::Container::Ptr
    DefaultLayoutBuilderHelper::getContentLayout(const BuilderProperties& props)
    {
        auto&& layout = tgui::Group::create(
            {
                "70%",
                uni::format(
                    "100% - {} - {} - {}",
                    props.titleHeight,
                    props.cornerButtonDimension,
                    props.cornerButtonPadding)
                    .c_str(),
            });
        layout->setPosition(
            { "parent.width / 2 - width / 2", props.titleHeight });
        return layout;
    }

    tgui::Container::Ptr DefaultLayoutBuilderHelper::getCornerButtonLayout(
        const BuilderProperties& props,
        tgui::HorizontalAlignment align,
        tgui::VerticalAlignment valign)
    {
        auto&& layout = tgui::Group::create(
            { props.cornerButtonDimension, props.baseHeight });
        const auto&& xPos = [&]() -> std::string
        {
            if (align == tgui::HorizontalAlignment::Right)
                return uni::format(
                    "parent.width - width - {}", props.cornerButtonPadding);
            return uni::format("{}", props.cornerButtonPadding);
        }();

        const auto&& yPos = [&]() -> std::string
        {
            if (valign == tgui::VerticalAlignment::Bottom)
                return uni::format(
                    "parent.height - height - {}", props.cornerButtonPadding);
            return uni::format("{}", props.cornerButtonPadding);
        }();

        layout->setPosition({ xPos.c_str(), yPos.c_str() });
        return layout;
    }

    tgui::Container::Ptr getTexturedTitleLayout(
        const BuilderProperties& props,
        const tgui::Texture& texture,
        tgui::HorizontalAlignment align)
    {
        auto&& layout = tgui::Group::create(
            { texture.getImageSize().x * props.titleHeight / texture.getImageSize().y,
              props.titleHeight });

        if (align == tgui::HorizontalAlignment::Center)
            layout->setPosition({ "parent.width / 2 - width / 2", "0%" });
        else if (align == tgui::HorizontalAlignment::Right)
            layout->setPosition({ "parent.width - width", "0%" });

        return layout;
    }
} // namespace priv
