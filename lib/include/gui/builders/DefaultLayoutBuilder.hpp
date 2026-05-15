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
        return BuilderRtnType(context); \
    } \
    \
    BuilderRtnType<StringId> methodName( \
        const tgui::Texture& texture, auto&& callback) \
    { \
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
        return BuilderRtnType(context); \
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
        const Sizer &sizer;
        const StringProvider<StringId> &strings;
        GuiAudioInterface &audioPlayer;
        BuilderProperties props;
        tgui::Container::Ptr content = tgui::Group::create();

        void addLayoutedContainer(tgui::Container::Ptr layout, tgui::Widget::Ptr container)
        {
            layout->add(container);
            content->add(layout);
        }
    };

    class [[nodiscard]] DefaultLayoutBuilderHelper final
    {
    public:
        static priv::BuilderProperties buildProperties(const Sizer& sizer);

        static tgui::Container::Ptr getContentLayout(const BuilderProperties& props);

        static tgui::Container::Ptr getCornerButtonLayout(
            const BuilderProperties& props,
            tgui::HorizontalAlignment align,
            tgui::VerticalAlignment valign);

        static tgui::Container::Ptr getTexturedTitleLayout(
            const BuilderProperties& props,
            const tgui::Texture& texture,
            tgui::HorizontalAlignment align);
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
        FinalizedLayoutBuilder<StringId> withNoBottomRightButton() const
        {
            return FinalizedLayoutBuilder(context);
        }

        GENERATE_CORNER_BUTTON_METHODS(
            FinalizedLayoutBuilder,
            withBottomRightButton,
            tgui::HorizontalAlignment::Right,
            tgui::VerticalAlignment::Bottom)

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
            tgui::VerticalAlignment::Bottom)

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
                DefaultLayoutBuilderHelper::getContentLayout(context.props),
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
                tgui::Group::create({ "100%", context.props.titleHeight }),
                WidgetBuilder::createHeading(
                    context.strings.getString(stringId), context.sizer, level));

            return LayoutBuilderWithBackgroundAndTitle(context);
        }

        LayoutBuilderWithBackgroundAndTitle<StringId>
        withTexturedTitle(
            const tgui::Texture& texture,
            tgui::HorizontalAlignment align = tgui::HorizontalAlignment::Center)
        {
            auto panel = tgui::Panel::create();
            panel->getRenderer()->setTextureBackground(texture);

            context.addLayoutedContainer(
                DefaultLayoutBuilderHelper::getTexturedTitleLayout(
                    context.props,
                    texture,
                    align),
                panel);

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
    DefaultLayoutBuilder(
        const Sizer& sizer,
        const StringProvider<StringId>& strings,
        GuiAudioInterface& audioPlayer) noexcept
        : context({
            .sizer = sizer,
            .strings = strings,
            .audioPlayer = audioPlayer,
            .props = priv::DefaultLayoutBuilderHelper::buildProperties(sizer),
        })
    {
    }

    DefaultLayoutBuilder(DefaultLayoutBuilder&&) = default;
    DefaultLayoutBuilder(const DefaultLayoutBuilder&) = delete;

public:
    priv::LayoutBuilderWithBackground<StringId>
    withBackgroundImage(const tgui::Texture& texture)
    {
        auto&& panel = tgui::Panel::create();
        panel->getRenderer()->setTextureBackground(texture);
        context.content = panel;
        return priv::LayoutBuilderWithBackground(context);
    }

    priv::LayoutBuilderWithBackground<StringId> withNoBackground()
    {
        return priv::LayoutBuilderWithBackground(context);
    }

private:
    priv::BuilderContext<StringId> context;
};
