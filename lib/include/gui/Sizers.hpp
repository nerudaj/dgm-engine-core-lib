#pragma once

#include <gui/HeadingLevel.hpp>

class [[nodiscard]] Sizer final
{
public:
    void setScale(float newScale)
    {
        scale = newScale;
    }

    [[nodiscard]] unsigned getBaseFontSize() const;

    [[nodiscard]] unsigned getBaseContainerHeight() const;

    [[nodiscard]] unsigned getFontSize(HeadingLevel level) const;

    [[nodiscard]] unsigned getContainerHeight(HeadingLevel level) const;

    [[nodiscard]] float getSizeMultiplier(HeadingLevel level) const;

private:
    float scale = 1.f;
};
