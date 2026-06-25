#pragma once

#include "audio/GuiAudioInterface.hpp"
#include "gui/Sizers.hpp"
#include "strings/StringProvider.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>
#include <optional>

struct [[nodiscard]] TabbedLayoutOptions final
{
    std::optional<std::string> tabsWidgetId = std::nullopt;
    std::optional<std::string> contentWidgetId = std::nullopt;
    bool contentIsScrollable = false;
    std::optional<std::string> contentClassName = std::nullopt;
    std::optional<std::string> tabsClassName = std::nullopt;
};

/**
 * Builder for creating a list of tabs and a content panel
 * which changes every time a tab is selected.
 *
 * Internally uses NavbarLayoutBuilder, therefore the tabs
 * are placed atop of the container..
 */
template<ScopedEnum StringId>
class [[nodiscard]] TabbedLayoutBuilder final
{
public:
    TabbedLayoutBuilder(
        const StringProvider<StringId>& strings,
        const Sizer& sizer,
        GuiAudioInterface& audioPlayer) noexcept
        : strings(strings), sizer(sizer), audioPlayer(audioPlayer)
    {
    }

    TabbedLayoutBuilder(const TabbedLayoutBuilder&) = delete;
    TabbedLayoutBuilder(TabbedLayoutBuilder&&) = default;
    ~TabbedLayoutBuilder() = default;

public:
    TabbedLayoutBuilder& addTab(
        const StringId stringId,
        const std::function<void(tgui::Container::Ptr)> onTabSelected,
        std::optional<std::string> className = std::nullopt)
    {
        const std::string tabName = strings.getString(stringId);
        tabNames.push_back(tabName);
        tabCallbacks[tabName] = onTabSelected;
        return *this;
    }

    TabbedLayoutBuilder& setTabSelected(const StringId stringId)
    {
        selectedTab = strings.getString(stringId);
        return *this;
    }

    [[nodiscard]] tgui::Container::Ptr
    build(const TabbedLayoutOptions& options = {})
    {
        auto content = createContentPanel(
            options.contentIsScrollable, options.contentClassName, sizer);
        auto tabs = WidgetBuilder::createTabs(
            tabNames,
            [tabNames = tabNames,
             callbacks = std::move(tabCallbacks),
             content = content](const tgui::String& tabName)
            {
                for (auto&& name : tabNames)
                {
                    if (name == tabName)
                    {
                        callbacks.at(name)(content);
                    }
                }
            },
            sizer,
            audioPlayer,
            WidgetOptions {
                .id = options.tabsWidgetId,
                .className = options.tabsClassName,
            });
        tabs->select(selectedTab);

        return NavbarLayoutBuilder(sizer)
            .withNavbarWidget(tabs)
            .withContent(content, options.contentWidgetId)
            .build();
    }

private:
    [[nodiscard]] static tgui::Container::Ptr createContentPanel(
        bool isScrollable,
        std::optional<std::string> className,
        const Sizer& sizer)
    {
        auto panel = [isScrollable = isScrollable]() -> tgui::Container::Ptr
        {
            if (isScrollable)
                return tgui::ScrollablePanel::create({ "100%", "100%" });
            return tgui::Panel::create({ "100%", "100%" });
        }();

        if (className)
        {
            panel->setRenderer(
                tgui::Theme::getDefault()->getRenderer(*className));
        }

#ifdef ANDROID
        if (isScrollable)
        {
            panel->cast<tgui::ScrollablePanel>()
                ->getRenderer()
                ->setScrollbarWidth(sizer.getBaseFontSize());
        }
#endif
        return panel;
    }

private:
    const StringProvider<StringId>& strings;
    const Sizer& sizer;
    GuiAudioInterface& audioPlayer;
    std::vector<std::string> tabNames;
    std::map<std::string, std::function<void(tgui::Container::Ptr)>>
        tabCallbacks;
    std::string selectedTab;
};
