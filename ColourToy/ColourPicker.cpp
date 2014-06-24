#include "stdafx.h"
#include "ColourPicker.h"
#include "Colour.h"
#include <vector>
#include <iostream>

#define THRESHOLD 200000

ColourPicker::ColourPicker()
{
    m_usedColour = new bool[256 * 256 * 256];
    for (int i = 0; i < 256 * 256 * 256; i++)
    {
        m_usedColour[i] = false;
        m_freeCount++;
    }
}

ColourPicker::~ColourPicker()
{
    delete [] m_usedColour;
}

void ColourPicker::Pick(unsigned colour)
{
    m_usedColour[colour & 0x00ffffff] = true;
    m_freeCount--;
}

unsigned ColourPicker::PickNearestToAlt(unsigned colour)
{
    if (m_AltCandidates.begin() == m_AltCandidates.end())
    {
        std::cout << "Switched to alt picker" << std::endl;
        for (int idx = 0; idx < 256 * 256 * 256; idx++)
        {
            if (!m_usedColour[idx])
                m_AltCandidates.push_back(idx);
        }
    }

    auto col = m_AltCandidates.front();
    m_AltCandidates.pop_front();

    m_usedColour[col] = true;
    m_freeCount--;
    return col | 0xff000000;
}

unsigned ColourPicker::PickNearestTo(unsigned colour)
{
    if (m_freeCount < THRESHOLD)
    {
        return PickNearestToAlt(colour);
    }
    auto idx = colour & 0x00ffffff;

    int distance = 1;
    unsigned newIdx;

    std::vector<unsigned> candidates;

    while (true)
    {
        for (int dr = R(idx) - distance; dr < R(idx) + distance; dr++)
        {
            if (dr < 0 || dr > 255) continue;

            for (int dg = G(idx) - distance; dg < G(idx) + distance; dg++)
            {
                if (dg < 0 || dg > 255) continue;

                for (int db = B(idx) - distance; db < B(idx) + distance; db++)
                {
                    if (db < 0 || db > 255) continue;

                    newIdx = Colour(dr, dg, db) & 0x00ffffff;

                    if (!m_usedColour[newIdx])
                    {
                        candidates.push_back(newIdx);
                    }
                }
            }
        }

        auto s = candidates.size();
        if (s > 0)
        {
            auto c = s / 2;

            newIdx = candidates[c];
            m_usedColour[newIdx] = true;
            m_freeCount--;
            return newIdx | 0xff000000;
        }

        distance++;
    }
}