#pragma once
class ColourPicker
{
public:
    ColourPicker();
    ~ColourPicker();

    unsigned PickNearestTo(unsigned colour);

private:
    bool* m_usedColour;
};

