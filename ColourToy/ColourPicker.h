#pragma once
class ColourPicker
{
public:
    ColourPicker();
    ~ColourPicker();

    unsigned PickNearestTo(unsigned colour);
    void Pick(unsigned colour);

private:
    bool* m_usedColour;
};

