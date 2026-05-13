#pragma once

#include "gui/HeadingLevel.hpp"
#include "gui/Sizers.hpp"
#include "gui/TguiHelper.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include "strings/StringProvider.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "audio/GuiAudioInterface.hpp"

namespace priv
{
    #define GENERATE_CORNER_BUTTON_METHODS(BuilderRtnType, methodName, align, valign) \
    BuilderRtnType<StringId> \
    methodName(StringId labelStringId, auto&& callback) \
    { \
        context.addLayoutedContainer( \
            DefaultLayoutBuilderHelper::getCornerButtonLayout( \
                context.props, \
                align, \
                valign), \
            WidgetBuilder::createButton( \
                context.strings.getString(labelStringId), \
                std::forward<decltype(callback)>(callback), \
                context.sizer, \
                context.audioPlayer)); \
        return LayoutBuilderWithContentAndOneButton(context); \
    }

    BuilderRtnType<StringId> methodName( \
        const tgui::Texture::Ptr& texture, auto&& callback) \
    { \
        /* TODO: layout */ \
        context.addLayoutedContainer( \
            DefaultLayoutBuilderHelper::getCornerButtonLayout( \
                context.props, \
                align, \
                valign), \
            WidgetBuilder::createTexturedButton( \
                texture, \
                std::forward<decltype(callback)>(callback), \
                context.sizer, \
                context.audioPlayer)); \
        return LayoutBuilderWithContentAndOneButton(context); \
    }

    struct [[nodiscard]] BuilderProperties
    {
        unsigned baseHeight;
        unsigned cornerButtonDimension;
        unsigned cornerButtonPadding;
        unsigned titleHeight;
    };

    template<ScopedEnum StringId>
    struct [[nodiscard]] BuilderContext
    {
        Sizer &sizer;
        StringProvider<StringId> &strings;
        GuiAudioInterface &audioPlayer;
        BuilderProperties props;
        tgui::Container::Ptr content = tgui::Group::create();

        void addLayoutedContainer(tgui::Container::Ptr layout, tgui::Container::Ptr container)
        {
            layout->add(container);
            content->add(layout);
        }
    };

    class [[nodiscard]] DefaultLayoutBuilderHelper final
    {
    public:
        static priv::BuilderProperties buildProperties(const Sizer& sizer);
        {
            layout->add(container);
            target->add(layout;)
            return target;
        }

        static tgui::Container::Ptr getContentLayout(const BuilderProperties& props);

        static tgui::Container::Ptr getCornerButtonLayout(
            const BuilderProperties& props,
            tgui::HorizontalAlignment align,
            tgui::VerticalAlignment valign);
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] FinalizedLayoutBuilder final
    {
    public:
        FinalizedLayoutBuilder(const BuilderContext<StringId>& context)
            : context(context)
        {
        }

    public:
        [[nodiscard]] inline tgui::Container::Ptr build() const
        {
            return context.content;
        }

    private:
        BuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] LayoutBuilderWithContentAndThreeButtons final
    {
    public:
        LayoutBuilderWithContentAndThreeButtons(const BuilderContext<StringId>& context)
            : context(context)
        {
        }

    public:
        FinalizedLayoutBuilder<String> withNoBottomRightButton() const
        {
            return FinalizedLayoutBuilder(context);
        }

        GENERATE_CORNER_BUTTON_METHODS(
            FinalizedLayoutBuilder,
            withBottomRightButton,
            tgui::HorizontalAlignment::Right,
            tgui::VerticalAlignment::Botton)

    private:
        BuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] LayoutBuilderWithContentAndTwoButtons final
    {
    public:
        LayoutBuilderWithContentAndTwoButtons(const BuilderContext<StringId>& context)
            : context(context)
        {
        }

    public:
        LayoutBuilderWithContentAndThreeButtons<StringId> withNoBottomLeftButton() const
        {
            return LayoutBuilderWithContentAndThreeButtons(context);
        }

        GENERATE_CORNER_BUTTON_METHODS(
            LayoutBuilderWithContentAndThreeButtons,
            withBottomLeftButton,
            tgui::HorizontalAlignment::Left,
            tgui::VerticalAlignment::Botton)

    private:
        BuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] LayoutBuilderWithContentAndOneButton final
    {
    public:
        LayoutBuilderWithContentAndOneButton(const BuilderContext<StringId>& context)
            : context(context)
        {
        }

    public:
        LayoutBuilderWithContentAndTwoButtons<StringId> withNoTopRightButton() const
        {
            return LayoutBuilderWithContentAndTwoButtons(context);
        }

        GENERATE_CORNER_BUTTON_METHODS(
            LayoutBuilderWithContentAndTwoButtons,
            withTopRightButton,
            tgui::HorizontalAlignment::Right,
            tgui::VerticalAlignment::Top)

    private:
        BuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] LayoutBuilderWithContent final
    {
    public:
        LayoutBuilderWithContent(const BuilderContext<StringId>& context)
            : context(context)
        {
        }

    public:
        LayoutBuilderWithContentAndOneButton<StringId> withNoTopLeftButton() const
        {
            return LayoutBuilderWithContentAndOneButton(context);
        }

        GENERATE_CORNER_BUTTON_METHODS(
            LayoutBuilderWithContentAndOneButton,
            withTopLeftButton,
            tgui::HorizontalAlignment::Left,
            tgui::VerticalAlignment::Top)

        FinalizedLayoutBuilder<StringId> withNoCornerButtons()
        {
            return FinalizedLayoutBuilder(context);
        }

    private:
        BuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] LayoutBuilderWithBackgroundAndTitle final
    {
    public:
        LayoutBuilderWithBackgroundAndTitle(const BuilderContext<StringId>& context)
            : context(context)
        {
        }

    public:
        LayoutBuilderWithContent<StringId> withContent(tgui::Container::Ptr content)
        {
            context.addLayoutedContainer(
                DefaultLayoutBuilderHelper::getLayoutForContent(props)
                content);
            return LayoutBuilderWithContent<StringId>(context);
        }

    private:
        BuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] LayoutBuilderWithBackground final
    {
    public:
        LayoutBuilderWithBackground(const BuilderContext<StringId>& context)
            : context(context)
        {
        }

    public:
        LayoutBuilderWithBackgroundAndTitle<StringId>
        withTitle(StringId stringId, HeadingLevel level)
        {
            context.addLayoutedContainer(
                tgui::Group::create({ "100%", props.titleHeight }),
                WidgetBuilder::createHeading(
                    strings.getString(stringId), sizer, level)));

            return LayoutBuilderWithBackgroundAndTitle(context);
        }

        LayoutBuilderWithBackgroundAndTitle<StringId>
        withTexturedTitle(const sf::Texture& texture)
        {
            // TODO: refactor
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

            context.addLayoutedContainer(panelContainer, panel);

            return LayoutBuilderWithBackgroundAndTitle(context);
        }

        LayoutBuilderWithBackgroundAndTitle<StringId> withNoTitle()
        {
            return LayoutBuilderWithBackgroundAndTitle(context);
        }

    private:
        BuilderContext<StringId> context;
    };
} // namespace priv

template<ScopedEnum StringId>
class [[nodiscard]] DefaultLayoutBuilder final
{
public:
    constexpr explicit DefaultLayoutBuilder(
        const Sizer& sizer,
        const StringProvider<StringId>& strings,
        const GuiAudioInterface& audioPlayer) noexcept
        : context({
            .sizer = sizer,
            .strings = strings,
            .audioPlayer = audioPlayer,
            .props = priv::DefaultLayoutBuilderHelper::(sizer),
        })
    {
    }

public:
    priv::LayoutBuilderWithBackground<StringId>
    withBackgroundImage(const sf::Texture& texture)
    {
        context.content = tgui::Panel::create();
        context.content->getRenderer()->setTextureBackground(
            TguiHelper::convertTexture(texture));
        return priv::LayoutBuilderWithBackground(context);
    }

    priv::LayoutBuilderWithBackground<StringId> withNoBackground()
    {
        return priv::LayoutBuilderWithBackground(context);
    }

private:
    BuilderContext<StringId> context;
};
