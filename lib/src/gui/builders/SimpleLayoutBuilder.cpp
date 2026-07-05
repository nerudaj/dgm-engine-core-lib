#include "gui/builders/SimpleLayoutBuilder.hpp"
#include "misc/Compatibility.hpp"

tgui::Container::Ptr
priv::SimpleLayoutBuilderHelper::createRootContainer(const Sizer& sizer)
{
    const auto sizeStr = uni::format("100% - 2 * {}", sizer.getBaseFontSize());
    auto layout = tgui::Group::create({ sizeStr.c_str(), sizeStr.c_str() });
    TguiHelper::centerInParent(layout);
    return layout;
}

tgui::Container::Ptr priv::SimpleLayoutBuilderHelper::createTitleContainer(
    const bool usePanel,
    const Sizer& sizer,
    const HeadingLevel level,
    const std::optional<std::string> panelClassName)
{
    const auto layout =
        tgui::Layout2d { "100%", sizer.getContainerHeight(level) };

    if (!usePanel) return tgui::Group::create(layout);

    auto panel = tgui::Panel::create(layout);

    if (panelClassName)
    {
        panel->setRenderer(
            tgui::Theme::getDefault()->getRenderer(*panelClassName));
    }

    return panel;
}

tgui::Container::Ptr priv::SimpleLayoutBuilderHelper::createContentContainer(
    const Sizer& sizer,
    const unsigned titleHeight,
    tgui::Container::Ptr actualContent)
{
    auto layout = tgui::Group::create({
        "100%",
        uni::format(
            "100% - {} - 2 * {} - {}",
            titleHeight,
            sizer.getBaseFontSize(),
            sizer.getBaseContainerHeight())
            .c_str(),
    });
    layout->setPosition({
        "0%",
        uni::format("{} + {}", titleHeight, sizer.getBaseFontSize()).c_str(),
    });
    layout->add(actualContent);
    return layout;
}

tgui::Container::Ptr priv::SimpleLayoutBuilderHelper::createBottomButton(
    const std::string& string,
    std::function<void()>&& callback,
    const WidgetOptions& options,
    const tgui::HorizontalAlignment alignment,
    const Sizer& sizer,
    GuiAudioInterface& audioPlayer,
    const std::optional<std::string> widthOverride)
{
    auto layout = tgui::Group::create();
    if (widthOverride)
    {
        layout->setSize(
            { widthOverride->c_str(), sizer.getBaseContainerHeight() });
    }
    else
    {
        layout->setSize({ 4 * sizer.getBaseContainerHeight(),
                          sizer.getBaseContainerHeight() });
    }

    TguiHelper::alignInParent(
        layout, alignment, tgui::VerticalAlignment::Bottom);

    layout->add(WidgetBuilder::createButton(
        string,
        std::forward<std::function<void()>>(callback),
        sizer,
        audioPlayer,
        options));

    return layout;
}
