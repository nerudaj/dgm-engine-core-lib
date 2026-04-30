#pragma once

#include "gui/HeadingLevel.hpp"
#include "gui/Sizers.hpp"
#include "strings/StringProvider.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace priv
{
    struct [[nodiscard]] BuilderProperties
    {
        unsigned baseHeight;
        unsigned cornerButtonDimension;
        unsigned cornerButtonPadding;
        unsigned titleHeight;
    };

    class [[nodiscard]] FinalizedLayoutBuilder final
    {
    public:
        FinalizedLayoutBuilder(
            tgui::Container::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        [[nodiscard]] inline tgui::Container::Ptr build() const
        {
            return container;
        }

    private:
        tgui::Container::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContentAndThreeButtons final
    {
    public:
        LayoutBuilderWithContentAndThreeButtons(
            tgui::Container::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        FinalizedLayoutBuilder withNoBottomRightButton() const
        {
            return FinalizedLayoutBuilder(container, props);
        }

        FinalizedLayoutBuilder withBottomRightButton(tgui::Button::Ptr button);

    private:
        tgui::Container::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContentAndTwoButtons final
    {
    public:
        LayoutBuilderWithContentAndTwoButtons(
            tgui::Container::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContentAndThreeButtons withNoBottomLeftButton() const
        {
            return LayoutBuilderWithContentAndThreeButtons(container, props);
        }

        LayoutBuilderWithContentAndThreeButtons
        withBottomLeftButton(tgui::Button::Ptr button);

    private:
        tgui::Container::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContentAndOneButton final
    {
    public:
        LayoutBuilderWithContentAndOneButton(
            tgui::Container::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContentAndTwoButtons withNoTopRightButton() const
        {
            return LayoutBuilderWithContentAndTwoButtons(container, props);
        }

        LayoutBuilderWithContentAndTwoButtons
        withTopRightButton(tgui::Button::Ptr button);

    private:
        tgui::Container::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContent final
    {
    public:
        LayoutBuilderWithContent(
            tgui::Container::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContentAndOneButton withNoTopLeftButton() const
        {
            return LayoutBuilderWithContentAndOneButton(container, props);
        }

        LayoutBuilderWithContentAndOneButton
        withTopLeftButton(tgui::Button::Ptr button);

        FinalizedLayoutBuilder withNoCornerButtons()
        {
            return FinalizedLayoutBuilder(container, props);
        }

    private:
        tgui::Container::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithBackgroundAndTitle final
    {
    public:
        LayoutBuilderWithBackgroundAndTitle(
            tgui::Container::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContent withContent(tgui::Container::Ptr content);

    private:
        tgui::Container::Ptr container;
        BuilderProperties props;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] LayoutBuilderWithBackground final
    {
    public:
        LayoutBuilderWithBackground(
            tgui::Container::Ptr container,
            const BuilderProperties& props,
            const Sizer& sizer,
            const StringProvider<StringId>& strings)
            : sizer(sizer), container(container), props(props), strings(strings)
        {
        }

    public:
        LayoutBuilderWithBackgroundAndTitle
        withTitle(StringId stringId, HeadingLevel level)
        {
            auto&& panel = tgui::Group::create({ "100%", props.titleHeight });
            panel->add(WidgetBuilder::createHeading(
                strings.getString(stringId), sizer, level));
            container->add(panel);
            return LayoutBuilderWithBackgroundAndTitle(container, props);
        }

        LayoutBuilderWithBackgroundAndTitle
        withTexturedTitle(const sf::Texture& texture)
        {
            tgui::Texture ttexture;
            ttexture.loadFromPixelData(
                texture.getSize(), texture.copyToImage().getPixelsPtr());

            auto&& panelContainer = tgui::Group::create(
                { texture.getSize().x * props.titleHeight / texture.getSize().y,
                  props.titleHeight });
            panelContainer->setPosition(
                { "parent.width / 2 - width / 2", "0%" });

            auto panel = tgui::Panel::create();
            panel->getRenderer()->setTextureBackground(ttexture);

            container->add(panelContainer);

            return LayoutBuilderWithBackgroundAndTitle(container, props);
        }

    private:
        const Sizer& sizer;
        const StringProvider<StringId>& strings;
        tgui::Container::Ptr container;
        BuilderProperties props;
    };
} // namespace priv

template<ScopedEnum StringId>
class [[nodiscard]] DefaultLayoutBuilder final
{
public:
    constexpr explicit DefaultLayoutBuilder(
        const Sizer& sizer, const StringProvider<StringId>& strings) noexcept
        : sizer(sizer), strings(strings)
    {
    }

public:
    priv::LayoutBuilderWithBackground
    withBackgroundImage(const sf::Texture& texture)
    {
        auto&& bgr = tgui::Panel::create();
        bgr->getRenderer()->setTextureBackground(
            TguiHelper::convertTexture(texture));
        return priv::LayoutBuilderWithBackground(
            bgr, buildProperties(sizer), sizer, strings);
    }

    priv::LayoutBuilderWithBackground withNoBackgroundImage()
    {
        return priv::LayoutBuilderWithBackground(
            tgui::Group::create(), buildProperties(sizer), sizer, strings);
    }

    // TODO: this
    priv::LayoutBuilderWithBackground withNoBackground();

private:
    static priv::BuilderProperties buildProperties(const Sizer& sizer)
    {
        const auto baseHeight = sizer.getBaseContainerHeight();

        return priv::BuilderProperties {
            .baseHeight = baseHeight,
            .cornerButtonDimension = baseHeight * 2,
            .cornerButtonPadding = baseHeight / 2,
            .titleHeight = 3 * baseHeight,
        };
    }

private:
    const Sizer& sizer;
    const StringProvider<StringId>& strings;
};
