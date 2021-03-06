#pragma once

#include <list>
#include <mutex>

class ColourPicker
{
public:
    ColourPicker();
    ~ColourPicker();

    unsigned PickNearestTo(unsigned colour);
    void Pick(unsigned colour);

    int MaxScannedThrough() { return m_maxScannedThrough; }

private:
    unsigned PickNearestToAlt(unsigned colour);
    std::list<unsigned> m_AltCandidates;

    std::mutex m_pickMutex;

    bool* m_usedColour;
    int m_freeCount;
    int m_maxScannedThrough;
};

