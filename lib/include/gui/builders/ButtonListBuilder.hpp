#pragma once

#include "audio/GuiAudioInterface.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include "misc/Compatibility.hpp"
#include "strings/StringProvider.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>
#include <string>
#include <vector>

struct [[nodiscard]] ButtonListOptions final
{
    tgui::HorizontalAlignment alignment = tgui::HorizontalAlignment::Center;
    tgui::VerticalAlignment vAlignment = tgui::VerticalAlignment::Center;
    std::string width =
#ifdef ANDROID
        "90%";
#else
        "50%";
#endif
    bool setFirstButtonFocused = true;
};

template<ScopedEnum StringId>
class [[nodiscard]] ButtonListBuilder final
{
public:
    ButtonListBuilder(
        const StringProvider<StringId>& strings,
        const Sizer& sizer,
        GuiAudioInterface& audioPlayer) noexcept
        : strings(strings), sizer(sizer), audioPlayer(audioPlayer)
    {
    }

    ButtonListBuilder(const ButtonListBuilder&) = delete;
    ButtonListBuilder(ButtonListBuilder&&) = delete;
    ~ButtonListBuilder() = default;

public:
    ButtonListBuilder& addButton(
        const StringId labelId,
        std::function<void(void)> onClick,
        const std::string& buttonId = "")
    {
        buttonProps.emplace_back(strings.getString(labelId), onClick, buttonId);
        return *this;
    }

    [[nodiscard]] tgui::Container::Ptr build(ButtonListOptions options = {})
    {
        auto&& layout = tgui::GrowVerticalLayout::create(options.width.c_str());

        const std::string& horizontalPosition = [&]
        {
            if (options.alignment == tgui::HorizontalAlignment::Center)
                return "parent.width / 2 - width / 2";
            else if (options.alignment == tgui::HorizontalAlignment::Right)
                return "parent.width - width";
            return "0%";
        }();
        layout->getRenderer()->setSpaceBetweenWidgets(
            static_cast<float>(sizer.getBaseFontSize()));

        std::vector<tgui::Button::Ptr> buttons;

        for (auto&& [idx, props] : std::views::enumerate(buttonProps))
        {
            auto group = tgui::Group::create(
                { "100%", sizer.getBaseContainerHeight() * 1.5f });
            auto&& button = WidgetBuilder::createButton(
                props.label, props.onClick, sizer, audioPlayer);
            group->add(button, props.buttonId);

            layout->add(group);
            buttons.push_back(button);
        }

        // Configure navigation and set first button focused
        for (size_t idx = 1u; idx < buttons.size() - 1; ++idx)
        {
            buttons[idx - 1]->setNavigationDown(buttons[idx]);
            buttons[idx]->setNavigationUp(buttons[idx - 1]);
        }
        buttons.front()->setFocused(options.setFirstButtonFocused);

        const std::string& verticalPosition = [&]
        {
            if (options.vAlignment == tgui::VerticalAlignment::Center)
                return "parent.height / 2 - height / 2";
            else if (options.vAlignment == tgui::VerticalAlignment::Bottom)
                return "parent.height - height";
            return "0%";
        }();

        layout->setPosition(
            { horizontalPosition.c_str(), verticalPosition.c_str() });

        return layout;
    }

private:
    struct ButtonProps
    {
        std::string label;
        std::function<void(void)> onClick;
        std::string buttonId;
    };

private:
    const StringProvider<StringId>& strings;
    const Sizer& sizer;
    GuiAudioInterface& audioPlayer;
    std::vector<ButtonProps> buttonProps;
};
