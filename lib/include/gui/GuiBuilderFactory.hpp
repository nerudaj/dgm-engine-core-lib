#pragma once

#include "audio/GuiAudioInterface.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/ButtonListBuilder.hpp"
#include "gui/builders/DefaultLayoutBuilder.hpp"
#include "gui/builders/FormBuilder.hpp"
#include "gui/builders/NavbarLayoutBuilder.hpp"
#include "gui/builders/TabbedLayoutBuilder.hpp"
#include "gui/builders/TableBuilder.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include "strings/StringProvider.hpp"

template<ScopedEnum StringId>
class [[nodiscard]] GuiBuilderFactory final
{
public:
    GuiBuilderFactory(
        const Sizer& sizer,
        const StringProvider<StringId>& strings,
        GuiAudioInterface& audioPlayer) noexcept
        : sizer(sizer), strings(strings), audioPlayer(audioPlayer)
    {
    }

    GuiBuilderFactory(GuiBuilderFactory&&) = delete;
    GuiBuilderFactory(const GuiBuilderFactory&) = delete;

public:
    [[nodiscard]] ButtonListBuilder<StringId> createButtonListBuilder() const
    {
        return ButtonListBuilder(strings, sizer, audioPlayer);
    }

    [[nodiscard]] DefaultLayoutBuilder createDefaultLayoutBuiler() const
    {
        return DefaultLayoutBuilder(sizer, strings);
    }

    [[nodiscard]] FormBuilder<StringId> createFormBuilder() const
    {
        return FormBuilder(strings, sizer);
    }

    [[nodiscard]] NavbarLayoutBuilder createNavbarLayoutBuilder() const
    {
        return NavbarLayoutBuilder(sizer);
    }

    [[nodiscard]] TabbedLayoutBuilder<StringId>
    createTabbedLayoutBuilder() const
    {
        return TabbedLayoutBuilder(strings, sizer, audioPlayer);
    }

    [[nodiscard]] TableBuilder createTableBuilder() const
    {
        return TableBuilder(sizer);
    }

    tgui::Button::Ptr createTextButton(StringId textId, auto&& callback)
    {
        return WidgetBuilder::createButton(
            strings.getString(textId),
            std::forward<decltype(callback)>(callback),
            sizer,
            audioPlayer);
    }

private:
    const Sizer& sizer;
    const StringProvider<StringId>& strings;
    GuiAudioInterface& audioPlayer;
};
