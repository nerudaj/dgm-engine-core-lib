#pragma once

#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include "strings/StringProvider.hpp"
#include <DGM/classes/Compatibility.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <string>
#include <tuple>
#include <vector>

template<ScopedEnum StringId>
class [[nodiscard]] FormBuilder final
{
public:
    FormBuilder(
        const StringProvider<StringId>& strings, const Sizer& sizer) noexcept
        : strings(strings), sizer(sizer)
    {
    }

    FormBuilder(const FormBuilder&) = delete;
    FormBuilder(FormBuilder&&) = delete;
    ~FormBuilder() = default;

public:
    FormBuilder& addLabel(const StringId labelId)
    {
        rowsToBuild.push_back(SectionLabelRow {
            .label = strings.getString(labelId),
        });
        return *this;
    }

    FormBuilder& addOption(
        const StringId labelId,
        tgui::Widget::Ptr widget,
        std::optional<std::string> widgetId = std::nullopt)
    {
        rowsToBuild.push_back(LabelWidgetRow {
            .label = strings.getString(labelId),
            .widget = widget,
            .widgetId = widgetId,
        });
        return *this;
    }

    [[deprecated(
        "Use regular addOption with optional widgetId argument")]] FormBuilder&
    addOptionWithWidgetId(
        const StringId labelId,
        tgui::Widget::Ptr widget,
        const std::string& widgetId)
    {
        rowsToBuild.push_back(LabelWidgetRow {
            .label = strings.getString(labelId),
            .widget = widget,
            .widgetId = widgetId,
        });
        return *this;
    }

    FormBuilder& addOptionWithSubmit(
        const StringId labelId,
        tgui::Widget::Ptr widget,
        tgui::Button::Ptr submitBtn)
    {
        rowsToBuild.push_back(LabelWidgetSubmitRow {
            .label = strings.getString(labelId),
            .widget = widget,
            .submitBtn = submitBtn,
        });
        return *this;
    }

    FormBuilder& addFullRowOption(
        tgui::Widget::Ptr widget,
        std::optional<std::string> widgetId = std::nullopt)
    {
        rowsToBuild.push_back(WidgetOnlyRow {
            .widget = widget,
            .widgetId = widgetId,
        });
        return *this;
    }

    FormBuilder& addSeparator()
    {
        rowsToBuild.push_back(HorizontalSeparatorRow {});
        return *this;
    }

    FormBuilder& addGap()
    {
        rowsToBuild.push_back(GapSeparatorRow {});
        return *this;
    }

    [[nodiscard]] tgui::Container::Ptr build()
    {
        auto&& verticalLayout = tgui::GrowVerticalLayout::create();
        verticalLayout->getRenderer()->setPadding({ 10.f, 10.f });

        for (auto&& row : rowsToBuild)
        {
            verticalLayout->add(std::visit(
                overloads {
                    [&](const HorizontalSeparatorRow&)
                    {
                        auto&& row = WidgetBuilder::createRow(sizer);
                        row->add(WidgetBuilder::createSeparator());
                        return row;
                    },
                    [&](const GapSeparatorRow&)
                    { return WidgetBuilder::createRow(sizer); },
                    [&](const SectionLabelRow& props)
                    {
                        auto&& row = WidgetBuilder::createRow(sizer);
                        row->add(WidgetBuilder::createTextLabel(
                            props.label, sizer, "justify"_true));
                        return row;
                    },
                    [&](const WidgetOnlyRow& props)
                    {
                        auto&& row = WidgetBuilder::createRow(sizer);
                        addToContainerWithOptionalId(
                            props.widget, row, props.widgetId);
                        return row;
                    },
                    [&](const LabelWidgetRow& row)
                    {
                        return createOptionRow(
                            row.label, row.widget, row.widgetId);
                    },
                    [&](const LabelWidgetSubmitRow& row)
                    {
                        return createOptionRowWithSubmitButton(
                            row.label, row.widget, row.submitBtn);
                    },
                },
                row));
        }

        return verticalLayout;
    }

private:
    [[nodiscard]] tgui::Container::Ptr createOptionRow(
        const std::string& labelText,
        tgui::Widget::Ptr widgetPtr,
        std::optional<std::string> widgetId)
    {
        assert(widgetPtr);

        auto&& row = WidgetBuilder::createRow(sizer);
        row->add(WidgetBuilder::createTextLabel(labelText, sizer));

        auto&& widgetPanel = tgui::Group::create({ "40%", "100%" });
        widgetPanel->setPosition("60%", "0%");
        row->add(widgetPanel);

        addToContainerWithOptionalId(widgetPtr, widgetPanel, widgetId);

        return row;
    }

    [[nodiscard]] tgui::Container::Ptr createOptionRowWithSubmitButton(
        const std::string& labelText,
        tgui::Widget::Ptr widgetPtr,
        tgui::Button::Ptr buttonPtr)
    {
        assert(widgetPtr);
        assert(buttonPtr);

        auto&& row = WidgetBuilder::createRow(sizer);
        row->add(WidgetBuilder::createTextLabel(labelText, sizer));

        auto&& widgetContainer = tgui::Group::create({ "25%", "100%" });
        widgetContainer->setPosition("60%", "0%");
        widgetContainer->add(widgetPtr);
        row->add(widgetContainer);

        auto&& btnContainer = tgui::Group::create({ "11%", "90%" });
        btnContainer->setPosition("87%", "5%");
        btnContainer->add(buttonPtr);
        row->add(btnContainer);

        return row;
    }

    void addToContainerWithOptionalId(
        tgui::Widget::Ptr widgetPtr,
        tgui::Container::Ptr containerPtr,
        std::optional<std::string> id)
    {
        id.or_else(
              [&]() -> std::optional<std::string>
              {
                  containerPtr->add(widgetPtr);
                  return std::nullopt;
              })
            .and_then(
                [&](auto id) -> std::optional<std::string>
                {
                    containerPtr->add(widgetPtr, id);
                    return id;
                });
    }

private:
    struct [[nodiscard]] HorizontalSeparatorRow final
    {
    };

    struct [[nodiscard]] GapSeparatorRow final
    {
    };

    struct [[nodiscard]] SectionLabelRow final
    {
        std::string label;
    };

    struct [[nodiscard]] WidgetOnlyRow final
    {
        tgui::Widget::Ptr widget;
        std::optional<std::string> widgetId = {};
    };

    struct [[nodiscard]] LabelWidgetRow final
    {
        std::string label;
        tgui::Widget::Ptr widget;
        std::optional<std::string> widgetId = {};
    };

    struct [[nodiscard]] LabelWidgetSubmitRow final
    {
        std::string label;
        tgui::Widget::Ptr widget;
        tgui::Button::Ptr submitBtn = {};
    };

    using RowType = std::variant<
        HorizontalSeparatorRow,
        GapSeparatorRow,
        SectionLabelRow,
        WidgetOnlyRow,
        LabelWidgetRow,
        LabelWidgetSubmitRow>;

    const StringProvider<StringId>& strings;
    const Sizer& sizer;
    std::vector<RowType> rowsToBuild;
};
