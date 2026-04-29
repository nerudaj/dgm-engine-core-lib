#pragma once

class GuiAudioInterface
{
public:
    virtual ~GuiAudioInterface() = default;

public:
    virtual void playClick() = 0;
};
