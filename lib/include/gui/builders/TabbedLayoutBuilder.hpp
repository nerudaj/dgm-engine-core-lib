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
        const std::function<void(tgui::Container::Ptr)> onTabSelected)
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
        auto content = createContentPanel(options.contentIsScrollable);
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
            });
        tabs->select(selectedTab);

        return NavbarLayoutBuilder(sizer)
            .withNavbarWidget(tabs)
            .withContent(content, options.contentWidgetId)
            .build();
    }

private:
    [[nodiscard]] static tgui::Container::Ptr
    createContentPanel(bool isScrollable) const
    {
        if (isScrollable)
        {
            return tgui::ScrollablePanel::create({ "100%", "100%" });
        }
        return tgui::Panel::create({ "100%", "100%" });
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
