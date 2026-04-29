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
        const StringProvider& strings,
        GuiAudioInterface& audioPlayer) noexcept
        : sizer(sizer), strings(strings), audioPlayer(audioPlayer)
    {
    }

    GuiBuilderFactory(GuiBuilderFactory&&) = delete;
    GuiBuilderFactory(const GuiBuilderFactory&) = delete;

public:
    ButtonListBuilder createButtonListBuilder() const
    {
        return ButtonListBuilder(strings, sizer, audioPlayer);
    }

    DefaultLayoutBuilder createDefaultLayoutBuiler() const
    {
        return DefaultLayoutBuilder(sizer);
    }

    FormBuilder createFormBuilder() const
    {
        return FormBuilder(strings, sizer);
    }

    NavbarLayoutBuilder createNavbarLayoutBuilder() const
    {
        return NavbarLayoutBuilder(sizer);
    }

    TabbedLayoutBuilder createTabbedLayoutBuilder() const
    {
        return TabbedLayoutBuilder(strings, sizer, audioPlayer);
    }

    TableBuilder createTableBuilder() const
    {
        return TableBuilder(sizer);
    }

private:
    const Sizer& sizer;
    const StringProvider<StringId>& strings;
    GuiAudioInterface& audioPlayer;
};
