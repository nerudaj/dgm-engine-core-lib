#pragma once

#include "audio/GuiAudioInterface.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include "strings/StringProvider.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace priv
{
    template<ScopedEnum StringId>
    struct [[nodiscard]] SimpleLayoutBuilderContext final
    {
        const Sizer& sizer;
        const StringProvider<StringId>& strings;
        GuiAudioInterface& audioPlayer;
        tgui::Container::Ptr root = nullptr;
        tgui::Container::Ptr content = nullptr;
        unsigned titleHeight = 0;
    };

    class [[nodiscard]] SimpleLayoutBuilderHelper final
    {
    public:
        static tgui::Container::Ptr createRootContainer(const Sizer& sizer);

        static tgui::Container::Ptr createTitleContainer(
            const bool usePanel,
            const Sizer& sizer,
            const HeadingLevel level,
            const std::optional<std::string> panelClassName = std::nullopt);
        
        static tgui::Container::Ptr createContentContainer(
            const Sizer& sizer,
            const unisgned titleHeight,
            tgui::Container::Ptr actualContent);
        
        static tgui::Container::Ptr createBottomButton(
            const std::string& string,
            std::function<void()>&& callback,
            const WidgetOptions& options,
            const tgui::HorizontalAlignment alignment,
            const Sizer& sizer,
            const GuiAudioInterface& audioPlayer,
            const std::optional<std::string> widthOverride);
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] SimpleLayoutFinalBuilder final
    {
    public:
        SimpleLayoutFinalBuilder(const SimpleLayoutBuilderContext<StringId>& context)
        : context(context) {}

    public:
        tgui::Container::Ptr build()
        {
            return context.root;
        }

    private:
        SimpleLayoutBuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] SimpleLayoutWithLeftButtonBuilder final
    {
    public:
        SimpleLayoutWithLeftButtonBuilder(const SimpleLayoutBuilderContext<StringId>& context)
        : context(context) {}

    public:
        SimpleLayoutFinalBuilder<StringId> withNoBottomLeftButton()
        {
            return SimpleLayoutFinalBuilder<StringId>(context);
        }

        SimpleLayoutFinalBuilder<StringId> withButtonLeftButton(
            StringId stringId,
            std::function<void()>&& callback,
            const WidgetOptions options = {},
            const std::optional<std::string> widthOverride = std::nullopt)
        {
            context.root->add(SimpleLayoutBuilderHelper::createBottomButton(
                context.strings.getString(stringId),
                std::forward<std::function<void()>>(callback),
                options,
                tgui::HorizontalAlignment::Right,
                context.sizer,
                context.audioPlayer,
                widthOverride));
            return SimpleLayoutFinalBuilder<StringId>(context);
        }

    private:
        SimpleLayoutBuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] SimpleLayoutWithContentBuilder final
    {
    public:
        SimpleLayoutWithContentBuilder(const SimpleLayoutBuilderContext<StringId>& context)
        : context(context) {}

    public:
        SimpleLayoutFinalBuilder<StringId> withNoBottomButtons()
        {
            return SimpleLayoutFinalBuilder<StringId>(context);
        }

        SimpleLayoutWithLeftButtonBuilder<StringId> withNoBottomLeftButton()
        {
            return SimpleLayoutWithLeftButtonBuilder<StringId>(context);
        }

        SimpleLayoutWithLeftButtonBuilder<StringId> withButtonLeftButton(
            StringId stringId,
            std::function<void()>&& callback,
            const WidgetOptions options = {},
            const std::optional<std::string> widthOverride = std::nullopt)
        {
            context.root->add(SimpleLayoutBuilderHelper::createBottomButton(
                context.strings.getString(stringId),
                std::forward<std::function<void()>>(callback),
                options,
                tgui::HorizontalAlignment::Left,
                context.sizer,
                context.audioPlayer,
                widthOverride));
            return SimpleLayoutWithLeftButtonBuilder<StringId>(context);
        }

    private:
        SimpleLayoutBuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] SimpleLayoutWithTitleBuilder final
    {
    public:
        SimpleLayoutWithTitleBuilder(const SimpleLayoutBuilderContext<StringId>& context)
        : context(context) {}

    public:
        SimpleLayoutWithContentBuilder<StringId> withContent(tgui::Container::Ptr content)
        {
            context.root->add(SimpleLayoutBuilderHelper::createContentContainer(
                context.sizer,
                context.titleHeight,
                content));
            return SimpleLayoutWithContentBuilder<StringId>(context);
        }

    private:
        SimpleLayoutBuilderContext<StringId> context;
    };

    template<ScopedEnum StringId>
    class [[nodiscard]] SimpleLayoutWithBackgroundBuilder final
    {
    public:
        SimpleLayoutWithBackgroundBuilder(const SimpleLayoutBuilderContext<StringId>& context)
        : context(context) {}

    public:
        SimpleLayoutWithTitleBuilder<StringId> withNoTitle()
        {
            return SimpleLayoutWithTitleBuilder<StringId>(context);
        }

        SimpleLayoutWithTitleBuilder<StringId> withTexturedTitle(
            const tgui::Texture& texture,
            const HeadingLevel level)
        {
            auto panel = Helper::createTitleContainer(context.sizer, level);
            panel->getRenderer()->setTextureBackground(texture);
            context.root->add(panel);
            return SimpleLayoutWithTitleBuilder<StringId>(context);
        }

        SimpleLayoutWithTitleBuilder<StringId> withTitleInPanel(
            StringId stringId, 
            const HeadingLevel level,
            tgui::HorizontalAlignment titleAlignment = tgui::HorizontalAlignment::Center,
            std::optional<std::string> panelClassName = std::nullopt)
        {
            return withTitle(
                stringId,
                level,
                titleAlignment,
                "usePanel"_true,
                panelClassName);
        }

        SimpleLayoutWithTitleBuilder<StringId> withPlainTitle(
            StringId stringId, 
            const HeadingLevel level,
            tgui::HorizontalAlignment titleAlignment = tgui::HorizontalAlignment::Center)
        {
            return withTitle(
                stringId,
                level,
                titleAlignment,
                "usePanel"_false,
                std::nullopt);
        }

    private:
        SimpleLayoutWithTitleBuilder<StringId> withTitle(
            StringId stringId, 
            const HeadingLevel level,
            tgui::HorizontalAlignment titleAlignment,
            bool usePanel,
            std::optional<std::string> panelClassName)
        {
            auto panel = Helper::createTitleContainer(
                context.sizer,
                level,
                usePanel,
                panelClassName);
            context.root->add(panel);
            
            panel->add(WidgetBuilder::createHeading(
                context.strings.getString(stringId),
                context.sizer,
                level,
                titleAlignment));
            context.titleHeight = sizer.getContainerHeight(level);

            return SimpleLayoutWithTitleBuilder<StringId>(context);
        }

    private:
        using Helper = SimpleLayoutBuilderHelper;

        SimpleLayoutBuilderContext<StringId> context;
    };
}

/**
 *  Simplified and slightly reworked version of DefaultLayoutBuilder
 *
 *  This one has configurable margins, title, main content,
 *  and bottom corner buttons.
 */
template<ScopedEnum StringId>
class [[nodiscard]] SimpleLayoutBuilder final
{
public:
    SimpleLayoutBuilder(
        const Sizer& sizer, 
        const StringProvider<StringId>& strings,
        GuiAudioInterface& audioPlayer)
        : context({
            .sizer = sizer,
            .strings = strings,
            .audioPlayer = audioPlayer,
            .root = priv::SimpleLayoutBuilderHelper::createRootContainer(sizer),
        })
    {}

public:
    priv::SimpleLayoutWithBackgroundBuilder<StringId>
    withBackgroundImage(const tgui::Texture& texture)
    {
        auto panel = tgui::Panel::create();
        panel->getRenderer()->setTextureBackground(texture);
        context.root->add(panel);
        return priv::SimpleLayoutWithBackgroundBuilder<StringId>(context);
    }

    priv::SimpleLayoutWithBackgroundBuilder<StringId>
    withNoBackground()
    {
        return priv::SimpleLayoutWithBackgroundBuilder<StringId>(context);
    }

private:
    SimpleLayoutBuilderContext<StringId> context;
};