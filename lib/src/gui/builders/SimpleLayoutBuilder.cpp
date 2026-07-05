#include <gui/builders/SimpleLayoutBuilder.hpp>

tgui::Container::Ptr
priv::SimpleLayoutBuilderHelper::createRootContainer(const Sizer& sizer)
{
    const auto sizeStr = uni::format("100% - 2 * {}", sizer.getBaseFontSizer());
    auto layout = tgui::Group::create({ sizeStr.c_str(), sizeStr.c_str() });
    layout->setPosition({ "parent.width / 2 - width / 2", "parent.height / 2 - height / 2" });
    return layout;
}

tgui::Container::Ptr
priv::SimpleLayoutBuilderHelper::createTitleContainer(
    const Sizer& sizer,
    const HeadingLevel level,
    const bool usePanel = true,
    const std::optional<std::string> panelClassName = std::nullopt)
{
    const auto layout = tgui::Layout2d{ "100%", sizer.getContainerHeight(level) };

    if (!usePanel)
        return tgui::Group::create(layout);

    auto panel = tgui::Panel::create(layout);
    
    if (panelClassName)
    {
        panel->setRenderer(tgui::Theme::getDefault()->getRenderer(*panelClassName));
    }

    return panel;
}

tgui::Container::Ptr
priv::SimpleLayoutBuilderHelper::createContentContainer(
    const Sizer& sizer,
    const unisgned titleHeight,
    tgui::Container::Ptr actualContent)
{
     auto layout = tgui::Group::create({
        "100%",
        uni::format(
            "100% - {} - 2 * {} - {}",
            titleHeight,
            sizer.getBaseFontSize(),
            sizer.getBaseContainerHeight()).c_str(),
        });
    layout->setPosition({
        "0%",
        uni::format(
            "{} + {}",
            titleHeight,
            sizer.getBaseFontSize()).c_str(),
        });
    layout->add(actualContent);
    return layout;
}

static tgui::Container::Ptr createBottomButton(
    const std::string& string,
    std::function<void()>&& callback,
    const WidgetOptions& options,
    const tgui::HorizontalAlignment alignment,
    const Sizer& sizer,
    const GuiAudioInterface& audioPlayer,
    const std::optional<std::string> widthOverride)
{
    auto layout = tgui::Group::create();
    if (widthOverride)
    {
        layout->setSize({ widthOverride->c_str(), sizer.getBaseContainerHeight() });
    }
    else
    {
        layout->setSize({ 3 * sizer.getBaseContainerHeight(), sizer.getBaseContainerHeight() });
    }

    layout->setPosition({
        alignment == tgui::HorizontalAlignment::Left
            ? "0%"
            : "parent.width - width",
        "0%"});

    layout->add(WidgetBuilder::createButton(
        string, std::forward<std::function<void()>>(callback), sizer, audioPlayer, options));

    return layout;
}