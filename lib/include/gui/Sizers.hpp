#pragma once

class [[nodiscard]] Sizer final
{
public:
    Sizer(Sizer&&) = default;
    Sizer(const Sizer&) = delete;

public:
    void setScale(float newScale)
    {
        scale = newScale;
    }

    [[nodiscard]] unsigned getBaseFontSize() const;

    [[nodiscard]] unsigned getBaseContainerHeight() const;

private:
    float scale = 1.f;
};
