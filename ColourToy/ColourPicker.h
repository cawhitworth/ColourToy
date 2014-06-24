#pragma once

#include <list>

class ColourPicker
{
public:
    ColourPicker();
    ~ColourPicker();

    unsigned PickNearestTo(unsigned colour);
    void Pick(unsigned colour);

private:
    unsigned PickNearestToAlt(unsigned colour);
    std::list<unsigned> m_AltCandidates;

    bool* m_usedColour;
    int m_freeCount;
};

